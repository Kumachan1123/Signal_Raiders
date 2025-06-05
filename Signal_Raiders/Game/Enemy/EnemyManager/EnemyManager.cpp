/*
*	@file	EnemyManager.cpp
*	@brief	敵マネージャークラス
*/
#include <pch.h>
#include "EnemyManager.h"
/*
*	@brief	コンストラクタ
*	@details 敵マネージャークラスのコンストラクタ
*	@param	CommonResources* pCommonResources 共通リソース
*	@return なし
*/
EnemyManager::EnemyManager(CommonResources* pCommonResources)
	: m_pCommonResources{ pCommonResources }// 共通リソース
	, m_pEnemies{}// 敵リスト
	, m_isEnemyBorn{ false }// 敵生成フラグ
	, m_isBorned{ false }// 敵生成済みフラグ
	, m_isHitPlayerToEnemy{ false }// プレイヤーと敵の当たり判定フラグ
	, m_isBossBorn{ false }// ボス生成フラグ
	, m_isBossBorned{ false }// ボス生成済みフラグ
	, m_isBossAlive{ true }// ボス生存フラグ
	, m_isBossAppear{ false }// ボス生成演出フラグ
	, m_enemyIndex{ 0 }// 敵の数
	, m_stageNumber{ 0 }// ステージ番号
	, m_enemyMax{ 0 }// 敵の生成上限
	, m_enemyBornInterval{ EnemyParameters::ENEMY_SPAWN_INTERVAL }// 敵の生成間隔
	, m_enemyBornTimer{ 0.0f }// 敵の生成タイマー
	, m_bossHP{ 0 }// ボスのHP
	, m_bossBulletType{ BossBase::BossBulletType::STAGE_1 }// ボスの弾の種類
	, m_specialAttackCount{ 0 }// ボスの特殊攻撃の数
	, m_startTime{ 0.0f }// ゲーム開始時間
	, m_bossBornWaitTime{ 0.0f }// ボス生成待機時間
	, m_pWifi{ nullptr }	// Wi-Fi
	, m_wifiThread{}	// Wi-Fiを取得するためのスレッド
	, m_pWall{ nullptr }// 壁
	, m_pPlayer{ nullptr }// プレイヤー
	, m_pBulletManager{ nullptr }// 弾マネージャー
	, m_SEVolume{ 0.0f }// SEの音量

{
	// Wi-Fiクラスを生成
	m_pWifi = std::make_unique<Wifi>();
}
/*
*	@brief	デストラクタ
*	@details 敵マネージャークラスのデストラクタ
*	@param	なし
*	@return なし
*/
EnemyManager::~EnemyManager() {/*do nothing*/ }
/*
*	@brief	初期化
*	@details 敵マネージャークラスの初期化
*	@param	Player* pPlayer プレイヤー
*	@return なし
*/
void EnemyManager::Initialize(Player* pPlayer)
{
	// プレイヤーのポインタを取得
	m_pPlayer = pPlayer;
	// 敵の生成上限設定
	SetEnemyMax();
}
/*
*	@brief	更新
*	@details 敵マネージャークラスの更新
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::Update(float elapsedTime)
{
	// ゲーム開始時間を更新
	UpdateStartTime(elapsedTime);
	// Wi-Fiの更新
	m_pWifi->Update(elapsedTime);
	// エフェクトの更新
	UpdateEffects(elapsedTime);
	// 敵の生成処理
	HandleEnemySpawning(elapsedTime);
	// 敵同士の当たり判定
	HandleEnemyCollisions();
	// 敵とプレイヤーの当たり判定
	UpdateEnemies(elapsedTime);
	// 敵と壁の当たり判定
	HandleWallCollision();
	// ザコ敵の削除処理
	RemoveDeadEnemies();
}
/*
*	@brief	描画
*	@details 敵マネージャークラスの描画
*	@param	なし
*	@return なし
*/
void EnemyManager::Render()
{
	using namespace DirectX::SimpleMath;
	// ビュー行列取得
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	// プロジェクション行列
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	// 全ての敵を描画する
	if (m_pEnemies.size() > 0)for (const auto& enemy : m_pEnemies)
	{
		// 敵を描画する
		enemy->Render(view, projection);
#ifdef _DEBUG// デバッグモードなら
		//	当たり判定を描画する
		enemy->DrawCollision(view, projection);
#endif
	}
	// エフェクトを描画する
	GetEffects().erase(std::remove_if(GetEffects().begin(), GetEffects().end(),
		[&](const std::unique_ptr<Effect>& pEffect)// 再生終了したパーティクルを削除する
		{
			// 再生終了したパーティクルは削除する
			if (!pEffect->IsPlaying()) return true;
			// パーティクルを描画する
			pEffect->Render(view, projection);
			//	再生中のパーティクルは削除しない
			return false;
		}),
		GetEffects().end()// 削除対象のパーティクルを削除する
	);
}
/*
*	@brief	敵の生成上限設定
*	@details ステージデータから敵の生成上限を設定する
*	@param	なし
*	@return なし
*/
void EnemyManager::SetEnemyMax()
{
	// ステージデータを取得
	auto it = stageData.find(m_stageNumber);
	// ステージデータが存在したら
	if (it != stageData.end())
	{
		// 敵の生成上限を設定
		m_enemyMax = it->second.enemyMax;
		// ボスのHPを設定
		m_bossHP = it->second.bossHP;
		// ボスの特殊攻撃の数を設定
		m_specialAttackCount = it->second.specialAttackCount;
		// ボスの弾の種類を設定
		m_bossBulletType = it->second.bulletType;
	}
}
/*
*	@brief	経過時間を加算
*	@details 敵生成開始時間を更新する
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::UpdateStartTime(float elapsedTime)
{
	// ゲーム開始時間を更新
	m_startTime += elapsedTime;
}
/*
*	@brief	全エフェクトの更新
*	@details エフェクトの更新を行う
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::UpdateEffects(float elapsedTime)
{
	// 全てのエフェクトを更新する
	for (auto& effect : GetEffects())effect->Update(elapsedTime);
}
/*
*	@brief	敵生成処理
*	@details 敵の生成処理を行う
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	// 敵生成タイマーを更新
	m_enemyBornTimer += elapsedTime;
	// 敵の数を取得
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());
	// 敵の数が敵の生成上限を超えたら敵の生成上限に設定
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;
	// ゲーム開始時間が敵生成開始時間を超えたらザコ敵を生成可能にする
	if (m_startTime >= EnemyParameters::ENEMY_SPAWN_START_TIME) m_isEnemyBorn = true;
	// ザコ敵生成可能かつザコ敵生成完了していない場合
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)
	{
		// 敵生成間隔を超えたら
		if (m_enemyBornTimer >= m_enemyBornInterval)
		{
			// 敵の種類を取得
			EnemyType enemyType = static_cast<EnemyType>(m_pWifi->GetEnemyTypes()[m_enemyIndex]);
			// 敵を生成(指定した敵の種類)
			SpawnEnemy(enemyType);
		}
	}
	// 生成上限に達したら敵生成完了処理を行う
	if (m_enemyIndex >= enemyNum)FinalizeEnemySpawn();
	// 敵がいなくなったらボスを生成
	if (m_pEnemies.empty() && m_isBorned && !m_isBossBorned)
	{
		// ボス生成演出フラグを立てる
		m_isBossAppear = true;
		// ボス生成待機時間を更新
		m_bossBornWaitTime += elapsedTime;
		// ボス生成待機時間を超えたらボスを生成
		if (m_bossBornWaitTime >= EnemyParameters::BOSS_SPAWN_WAIT_TIME)SpawnBoss();
	}
}
/*
*	@brief	敵生成処理
*	@details 指定した敵の種類を生成する
*	@param	EnemyType type 敵の種類
*	@return なし
*/
void EnemyManager::SpawnEnemy(EnemyType type)
{
	// ファクトリで敵を生成
	auto enemy = EnemyFactory::CreateEnemy(type, m_pPlayer, m_pCommonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	// 敵を初期化
	enemy->Initialize();
	// 弾マネージャーを設定
	enemy->SetBulletManager(m_pBulletManager);
	// 敵リストに追加
	m_pEnemies.push_back(std::move(enemy));
	// 敵生成タイマーを初期化
	m_enemyBornTimer = 0.0f;
	// 敵インデックスを増加
	m_enemyIndex++;
}
/*
*	@brief	敵生成完了処理
*	@details 敵生成完了処理を行う
*	@param	なし
*	@return なし
*
*/
void EnemyManager::FinalizeEnemySpawn()
{
	// 敵生成タイマーを初期化
	m_enemyBornTimer = 0.0f;
	// ザコ敵を生成不可能にする
	m_isEnemyBorn = false;
	// ザコ敵生成完了
	m_isBorned = true;
}
/*
*	@brief	ボス生成処理
*	@details ボスを生成する
*	@param	なし
*	@return なし
*/
void EnemyManager::SpawnBoss()
{
	// ボスを生成（プレイヤー、共通リソース、HPを引数にして作成）
	auto boss = std::make_unique<BossBase>(m_pPlayer, m_pCommonResources, m_bossHP);
	// ステージ番号が3以上なら「ラスボス」、それ以外は「通常ボス」として設定
	if (m_stageNumber >= 3)// ラスボスに設定
		boss->SetBossType(BossType::LAST_BOSS);
	else// 通常ボスに設定
		boss->SetBossType(BossType::NORMAL_BOSS);
	// 弾マネージャーをボスに渡す
	boss->SetBulletManager(m_pBulletManager);
	// ボスの弾の種類を設定
	boss->SetBulletType(m_bossBulletType);
	// 弾マネージャーに、ボス専用の特殊攻撃の回数を伝える
	boss->GetBulletManager()->SetSpecialAttackCount(m_specialAttackCount);
	// ボスの初期化処理
	boss->Initialize();
	// 敵リストにボスを追加
	m_pEnemies.push_back(std::move(boss));
	// ボスが出現したフラグをONにする
	m_isBossBorned = true;
	// ボス登場演出中フラグをOFFにする
	m_isBossAppear = false;
}
/*
*	@brief	敵同士の当たり判定処理
*	@details 敵同士の当たり判定を処理する
*	@param	なし
*	@return なし
*/
void EnemyManager::HandleEnemyCollisions()
{
	using namespace DirectX::SimpleMath;
	// 登録されている敵の数だけループ
	for (size_t i = 0; i < m_pEnemies.size(); ++i)
	{
		// i番目の敵以降の敵と衝突チェック
		for (size_t j = i + 1; j < m_pEnemies.size(); ++j)
		{
			// 同じインデックス同士は判定しない
			if (i == j) { continue; }
			// 敵Aと敵Bの当たり判定
			bool hit = m_pEnemies[i]->GetBoundingSphere().Intersects(m_pEnemies[j]->GetBoundingSphere());
			// 両方の敵に当たってるかどうかのフラグを設定
			m_pEnemies[i]->SetHitToOtherEnemy(hit);
			m_pEnemies[j]->SetHitToOtherEnemy(hit);
			// 衝突していたら位置の押し戻し処理を行う
			if (hit)
			{
				// 敵Aの当たり判定を取得
				auto& enemyA = m_pEnemies[i]->GetBoundingSphere();
				// 敵Bの当たり判定を取得
				auto& enemyB = m_pEnemies[j]->GetBoundingSphere();
				// 敵Aの新しい位置を計算
				Vector3 newPos = CheckHitOtherObject(enemyA, enemyB);
				// 敵Aの位置を更新
				m_pEnemies[i]->SetPosition(newPos);
			}
		}
	}
}
/*
*	@brief	敵と壁の当たり判定処理
*	@details 敵と壁の当たり判定を処理する
*	@param	なし
*	@return なし
*/
void EnemyManager::HandleWallCollision()
{
	using namespace DirectX::SimpleMath;
	// 登録されている敵の数だけループ
	for (auto& enemy : m_pEnemies)
	{
		// 壁の数だけループ
		for (int i = 0; i < m_pWall->GetWallNum(); i++)
		{
			// 敵の当たり判定と壁の当たり判定が交差したら
			if (enemy->GetBoundingSphere().Intersects(m_pWall->GetBoundingBox(i)))
			{
				// 壁に当たったら押し戻す
				Vector3 newPos = CheckHitWall(enemy->GetBoundingSphere(), m_pWall->GetBoundingBox(i));
				// 敵の位置を設定
				enemy->SetPosition(newPos);
			}
		}
	}
}
/*
*	@brief	敵全体の更新処理
*	@details 敵の更新処理を行う
*	@param	float elapsedTime 経過時間
* 	@return なし
*/
void EnemyManager::UpdateEnemies(float elapsedTime)
{
	// 攻撃中の敵配列をクリア
	m_pAttackingEnemies.clear();
	// 登録されている敵の数だけループ
	for (auto& enemy : m_pEnemies)
	{
		// 敵の更新
		enemy->Update(elapsedTime);
		// 敵の弾がプレイヤーに当たった場合の処理
		HandleEnemyBulletCollision(enemy);
		// 敵がプレイヤーに当たった場合の処理
		HandleEnemyPlayerCollision(enemy);
	}
}
/*
*	@brief	敵の弾とプレイヤーの当たり判定処理
*	@details 敵の弾がプレイヤーに当たったかどうかを処理する
*	@param	std::unique_ptr<IEnemy>& pEnemy 敵のポインタ
*	@return なし
*/
void EnemyManager::HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& pEnemy)
{
	// 敵の弾がプレイヤーに当たったかどうかを取得
	bool hit = pEnemy->GetPlayerHitByEnemyBullet();
	// 敵弾がプレイヤーに当たったら
	if (hit)
	{
		// 新しいプレイヤーのHPを計算
		float playerHP = m_pPlayer->GetPlayerHP() - pEnemy->GetToPlayerDamage();
		// 新しいHPを設定
		m_pPlayer->SetPlayerHP(playerHP);
		// プレイヤーのHPを減少したらフラグを下ろす
		pEnemy->SetPlayerHitByEnemyBullet(false);
		// プレイヤーが負傷した音を再生する
		m_pCommonResources->GetAudioManager()->PlaySound("Damage", m_pPlayer->GetVolume());
	}
}
/*
*	@brief	敵とプレイヤーの当たり判定処理
*	@details 敵がプレイヤーに当たったかどうかを処理する
*	@param	std::unique_ptr<IEnemy>& pEnemy 敵のポインタ
*	@return なし
*/
void EnemyManager::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& pEnemy)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ヒットフラグを初期化
	m_isHitPlayerToEnemy = false;
	// 敵の当たり判定とプレイヤーの当たり判定が交差したら
	if (pEnemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		// ヒットフラグを立てる
		m_isHitPlayerToEnemy = true;
		// プレイヤーを感知する当たり判定を取得
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();
		// プレイヤーを感知する当たり判定を縮小し、プレイヤーの当たり判定とする
		playerSphere.Radius /= 3.0f;
		// 敵の当たり判定とプレイヤーの当たり判定が交差したら
		if (pEnemy->GetBoundingSphere().Intersects(playerSphere))
		{
			// 敵の当たり判定を取得　
			auto& enemySphere = pEnemy->GetBoundingSphere();
			// 押し戻し考慮
			Vector3 newPos = CheckHitOtherObject(enemySphere, playerSphere);
			// 敵の位置を設定
			pEnemy->SetPosition(newPos);
		}
	}
	// 敵がプレイヤーに当たったかどうかのフラグを設定
	pEnemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	// プレイヤーの当たり判定を設定
	pEnemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
}
/*
*	@brief	死亡した敵を削除
*	@details 敵のリストから死亡した敵を削除する
*	@param	なし
*	@return なし
*/
void EnemyManager::RemoveDeadEnemies()
{
	// remove_ifで削除すべき敵を後ろに寄せて、eraseでまとめて削除
	m_pEnemies.erase(
		std::remove_if(
			m_pEnemies.begin(),
			m_pEnemies.end(),
			[this](std::unique_ptr<IEnemy>& pEnemy)
			{
				// 敵が倒されていたら
				if (pEnemy->GetEnemyIsDead())
				{
					// 死亡処理を実行
					HandleEnemyDeath(pEnemy);

					// この敵は削除対象なのでtrueを返す
					return true;
				}
				// 生きている敵はそのまま残しておく
				return false;
			}
		),
		m_pEnemies.end() // remove_ifで後ろに寄せた削除対象をまとめて消す
	);

}

/*
*	@brief	敵の死亡処理
*	@details 敵が死亡した時の処理を行う
*	@param	std::unique_ptr<IEnemy>& pEnemy 敵のポインタ
*	@return なし
*/
void EnemyManager::HandleEnemyDeath(std::unique_ptr<IEnemy>& pEnemy)
{
	// エフェクトのスケールを設定するための変数
	float effectScale;
	// IEnemyからBossのポインターを抽出
	auto pBoss = dynamic_cast<BossBase*>(pEnemy.get());
	// もし抽出したポインターがボスだったら
	if (pBoss)
	{
		// ボスの場合はエフェクトのスケールを大きくする
		effectScale = pBoss->GetDeadEffectSize();
		// 生存フラグをfalseにする
		m_isBossAlive = false;
	}
	else // もしボスじゃなかったら
	{
		// ザコ敵の場合はエフェクトのスケールを小さくする
		effectScale = EnemyParameters::ENEMY_DEADEFFECT_SCALE;
	}
	// エフェクトを生成してリストに追加
	m_pEffects.push_back(std::make_unique<Effect>(m_pCommonResources,// エフェクトを生成
		Effect::EffectType::ENEMY_DEAD,// エフェクトの種類指定
		pEnemy->GetPosition(), // 座標設定
		effectScale,// スケール設定
		pEnemy->GetMatrix()));// ワールド行列作成
	// 敵の死亡音を再生
	m_pCommonResources->GetAudioManager()->PlaySound("EnemyDead", m_pPlayer->GetVolume() + m_pPlayer->GetVolumeCorrection());
}