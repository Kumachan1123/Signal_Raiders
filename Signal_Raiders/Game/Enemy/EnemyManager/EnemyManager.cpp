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
	m_pWifi = std::make_unique<Wifi>();	// Wi-Fiクラスを生成
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
	m_pPlayer = pPlayer;// プレイヤーのポインタを取得
	SetEnemyMax();// 敵の生成上限設定
}
/*
*	@brief	更新
*	@details 敵マネージャークラスの更新
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::Update(float elapsedTime)
{
	UpdateStartTime(elapsedTime);	// ゲーム開始時間を更新
	m_pWifi->Update(elapsedTime);	// Wi-Fiの更新
	UpdateEffects(elapsedTime);	// エフェクトの更新
	HandleEnemySpawning(elapsedTime);	// 敵の生成処理
	HandleEnemyCollisions();	// 敵同士の当たり判定
	UpdateEnemies(elapsedTime);	// 敵とプレイヤーの当たり判定
	HandleWallCollision();	// 敵と壁の当たり判定
	RemoveDeadEnemies();	// ザコ敵の削除処理
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
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();//	ビュー行列取得
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();//	プロジェクション行列
	if (m_pEnemies.size() > 0)for (const auto& enemy : m_pEnemies)//	全ての敵を描画する
	{
		enemy->Render(view, projection);//	敵を描画する
#ifdef _DEBUG// デバッグモードなら
		enemy->DrawCollision(view, projection);//	当たり判定を描画する
#endif
	}
	GetEffects().erase(std::remove_if(GetEffects().begin(), GetEffects().end(),	// エフェクトを描画する
		[&](const std::unique_ptr<Effect>& pEffect)//	再生終了したパーティクルを削除する
		{
			if (!pEffect->IsPlaying()) return true;// 再生終了したパーティクルは削除する
			pEffect->Render(view, projection);// パーティクルを描画する
			return false;//	再生中のパーティクルは削除しない
		}),
		GetEffects().end()//	削除対象のパーティクルを削除する
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
	auto it = stageData.find(m_stageNumber);// ステージデータを取得
	if (it != stageData.end())// ステージデータが存在したら
	{
		m_enemyMax = it->second.enemyMax;// 敵の生成上限を設定
		m_bossHP = it->second.bossHP;// ボスのHPを設定
		m_specialAttackCount = it->second.specialAttackCount;// ボスの特殊攻撃の数を設定
		m_bossBulletType = it->second.bulletType;// ボスの弾の種類を設定
	}
}
/*
*	@brief	経過時間を加算
*	@details 敵生成開始時間を更新する
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::UpdateStartTime(float elapsedTime) { m_startTime += elapsedTime; }
/*
*	@brief	全エフェクトの更新
*	@details エフェクトの更新を行う
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::UpdateEffects(float elapsedTime) { for (auto& effect : GetEffects())effect->Update(elapsedTime); }
/*
*	@brief	敵生成処理
*	@details 敵の生成処理を行う
*	@param	float elapsedTime 経過時間
*	@return なし
*/
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	m_enemyBornTimer += elapsedTime;	// 敵生成タイマーを更新
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());	// 敵の数を取得
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;	// 敵の数が敵の生成上限を超えたら敵の生成上限に設定
	// ゲーム開始時間が敵生成開始時間を超えたら
	if (m_startTime >= EnemyParameters::ENEMY_SPAWN_START_TIME) m_isEnemyBorn = true;// ザコ敵生成可能にする
	// ザコ敵生成可能かつザコ敵生成完了していない場合
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// 敵生成間隔を超えたら
		{
			EnemyType enemyType = static_cast<EnemyType>(m_pWifi->GetEnemyTypes()[m_enemyIndex]);// 敵の種類を取得
			SpawnEnemy(enemyType);// 敵を生成(指定した敵の種類)
		}
	}
	if (m_enemyIndex >= enemyNum)FinalizeEnemySpawn();// 生成上限に達したら敵生成完了処理
	if (m_pEnemies.empty() && m_isBorned && !m_isBossBorned)	// 敵がいなくなったらボスを生成
	{
		m_isBossAppear = true; // ボス生成演出フラグを立てる
		m_bossBornWaitTime += elapsedTime;// ボス生成待機時間を更新
		if (m_bossBornWaitTime >= EnemyParameters::BOSS_SPAWN_WAIT_TIME)// ボス生成待機時間を超えたら
			SpawnBoss();// ボスを生成
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
	auto enemy = EnemyFactory::CreateEnemy(type, m_pPlayer, m_pCommonResources,
		m_pWifi->GetWifiLevels()[m_enemyIndex]);// ファクトリで敵生成
	enemy->Initialize();// 敵を初期化
	enemy->SetBulletManager(m_pBulletManager);// 弾マネージャーを設定
	m_pEnemies.push_back(std::move(enemy));// 敵リストに追加
	m_enemyBornTimer = 0.0f;// 敵生成タイマーを初期化
	m_enemyIndex++; // 敵インデックスを増加
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
	m_enemyBornTimer = 0.0f; // 敵生成タイマーを初期化
	m_isEnemyBorn = false; // ザコ敵生成不可能にする
	m_isBorned = true; // ザコ敵生成完了
}
/*
*	@brief	ボス生成処理
*	@details ボスを生成する
*	@param	なし
*	@return なし
*/
void EnemyManager::SpawnBoss()
{
	auto boss = std::make_unique<BossBase>(m_pPlayer, m_pCommonResources, m_bossHP);// ボスを生成
	if (m_stageNumber >= 3) boss->SetBossType(BossType::LAST_BOSS);// ボスの種類を設定
	else boss->SetBossType(BossType::NORMAL_BOSS);// ボスの種類を設定
	boss->SetBulletManager(m_pBulletManager);// 弾マネージャーを設定
	boss->SetBulletType(m_bossBulletType);// ボスの弾の種類を設定
	boss->GetBulletManager()->SetSpecialAttackCount(m_specialAttackCount);// ボスの特殊攻撃の数を設定
	boss->Initialize();// ボスを初期化
	m_pEnemies.push_back(std::move(boss)); // ボスも enemies に統一
	m_isBossBorned = true; // ボス生成完了
	m_isBossAppear = false; // ボス生成演出フラグを下ろす
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
	for (size_t i = 0; i < m_pEnemies.size(); ++i)// 敵の数だけループ
	{
		for (size_t j = i + 1; j < m_pEnemies.size(); ++j)// 敵の数だけループ
		{
			if (i == j) { continue; } // 同じ敵同士は当たり判定しない
			bool hit = m_pEnemies[i]->GetBoundingSphere().Intersects(m_pEnemies[j]->GetBoundingSphere());// 当たり判定を取得
			m_pEnemies[i]->SetHitToOtherEnemy(hit);// 当たり判定結果を設定
			m_pEnemies[j]->SetHitToOtherEnemy(hit);// 当たり判定結果を設定
			if (hit)// 当たり判定があったら
			{
				// m_pEnemies[i]の新しい座標
				auto& enemyA = m_pEnemies[i]->GetBoundingSphere();// 敵Aの当たり判定を取得
				auto& enemyB = m_pEnemies[j]->GetBoundingSphere();// 敵Bの当たり判定を取得
				Vector3 newPos = CheckHitOtherObject(enemyA, enemyB);// 押し戻し考慮
				m_pEnemies[i]->SetPosition(newPos);// 敵Aの位置を設定
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
	for (auto& enemy : m_pEnemies)// 敵の数だけループ
	{
		for (int i = 0; i < m_pWall->GetWallNum(); i++)// 壁の数だけループ
		{
			if (enemy->GetBoundingSphere().Intersects(m_pWall->GetBoundingBox(i)))// 敵の当たり判定と壁の当たり判定が交差したら
			{
				Vector3 newPos = CheckHitWall(enemy->GetBoundingSphere(), m_pWall->GetBoundingBox(i));// 壁に当たったら押し戻す
				enemy->SetPosition(newPos);// 敵の位置を設定
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
	m_pAttackingEnemies.clear();// 攻撃中の敵配列をクリア
	for (auto& enemy : m_pEnemies)// 敵の数だけ
	{
		enemy->Update(elapsedTime);// 敵の更新
		HandleEnemyBulletCollision(enemy);// 敵の弾がプレイヤーに当たったら
		HandleEnemyPlayerCollision(enemy);// 敵がプレイヤーに当たったら
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
	bool hit = pEnemy->GetPlayerHitByEnemyBullet();// 敵の弾がプレイヤーに当たったかどうか
	if (hit)// 敵弾がプレイヤーに当たったら
	{
		float playerHP = m_pPlayer->GetPlayerHP() - pEnemy->GetToPlayerDamage();// プレイヤーのHPを減少
		m_pPlayer->SetPlayerHP(playerHP);// プレイヤーのHPを設定
		pEnemy->SetPlayerHitByEnemyBullet(false); // プレイヤーのHPを減少したらフラグを下ろす
		m_pCommonResources->GetAudioManager()->PlaySound("Damage", m_pPlayer->GetVolume()); // SEを再生
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
	m_isHitPlayerToEnemy = false; // フラグを初期化
	if (pEnemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))// 敵の当たり判定とプレイヤーの当たり判定が交差したら
	{
		m_isHitPlayerToEnemy = true;// フラグを立てる
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();// プレイヤーの当たり判定を取得
		playerSphere.Radius /= 3.0f;// プレイヤーの当たり判定を縮小
		// 敵がプレイヤーを認識する範囲 / 3.0f = プレイヤーの当たり判定の半径
		if (pEnemy->GetBoundingSphere().Intersects(playerSphere))// 敵の当たり判定とプレイヤーの当たり判定が交差したら
		{
			auto& enemySphere = pEnemy->GetBoundingSphere(); // 敵の当たり判定を取得　
			Vector3 newPos = CheckHitOtherObject(enemySphere, playerSphere); // 押し戻し考慮
			pEnemy->SetPosition(newPos); // 敵の位置を設定
		}
	}
	pEnemy->SetHitToPlayer(m_isHitPlayerToEnemy);	//　敵がプレイヤーに当たったかどうかのフラグを設定
	pEnemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere()); // プレイヤーの当たり判定を設定
}
/*
*	@brief	死亡した敵を削除
*	@details 敵のリストから死亡した敵を削除する
*	@param	なし
*	@return なし
*/
void EnemyManager::RemoveDeadEnemies()
{
	m_pEnemies.erase(std::remove_if(m_pEnemies.begin(), m_pEnemies.end(),		// 死亡した敵を削除する
		[this](std::unique_ptr<IEnemy>& pEnemy)
		{
			if (pEnemy->GetEnemyIsDead())// 敵が倒されたら
			{
				HandleEnemyDeath(pEnemy);// 死亡処理
				return true; // 削除
			}
			return false; // 残す
		}),
		m_pEnemies.end());// 削除対象の敵を削除する
}

/*
*	@brief	敵の死亡処理
*	@details 敵が死亡した時の処理を行う
*	@param	std::unique_ptr<IEnemy>& pEnemy 敵のポインタ
*	@return なし
*/
void EnemyManager::HandleEnemyDeath(std::unique_ptr<IEnemy>& pEnemy)
{
	float effectScale;// 初期値
	auto pBoss = dynamic_cast<BossBase*>(pEnemy.get());// IEnemyからBossのポインターを抽出
	if (pBoss)	// もしボスだったら
	{
		effectScale = pBoss->GetDeadEffectSize();// ボスの場合はエフェクトのスケールを大きくする
		m_isBossAlive = false; // 生存フラグをfalseにする
	}
	else // もしボスじゃなかったら
	{
		effectScale = EnemyParameters::ENEMY_DEADEFFECT_SCALE;// ザコ敵の場合はエフェクトのスケールを小さくする
	}
	m_pEffects.push_back(std::make_unique<Effect>(m_pCommonResources,// エフェクトを生成
		Effect::EffectType::ENEMY_DEAD,// エフェクトの種類指定
		pEnemy->GetPosition(), // 座標設定
		effectScale,// スケール設定
		pEnemy->GetMatrix()));// ワールド行列作成
	m_pCommonResources->GetAudioManager()->PlaySound("EnemyDead", m_pPlayer->GetVolume() + m_pPlayer->GetVolumeCorrection());// 敵のSEを再生

}