/*
*	@file	BulletManager.cpp
*	@brief	弾管理クラス
*/
#include "pch.h"
#include "BulletManager.h"
/*
*	@brief	コンストラクタ
*	@details	弾管理クラスの初期化
*	@param	commonResources	共通リソース
*/
BulletManager::BulletManager(CommonResources* commonResources)
	:m_pCommonResources(commonResources)// 共通リソース
	, m_pPlayer(nullptr) // プレイヤー
	, m_pEnemyManager(nullptr) // 敵管理
	, m_pShooter(nullptr) // 発射元
	, m_isPlayerShoot(false) // プレイヤーが撃ったかどうか
	, m_isReloading(false) // リロード中かどうか
	, m_enemyBulletType(BulletType::NORMAL) // 敵の弾の種類
	, m_enemyBulletSize(0.0f) // 敵の弾のサイズ
	, m_playerBulletCount(0) // プレイヤーの弾の数
	, m_reloadTimer(0.0f) // リロードタイマー
	, m_elapsedTime(0.0f) // 経過時間
	, m_specialAttackCount(0) // 特殊攻撃の数
{
}
/*
*	@brief	デストラクタ
*	@details	弾管理クラスの終了処理
*	@param	なし
*	@return なし
*/
BulletManager::~BulletManager()
{
	for (auto& bullet : m_pPlayerBullets)bullet.reset();	// プレイヤーの弾を削除
	m_pPlayerBullets.clear();
	for (auto& bullet : m_pEnemyBullets)bullet.reset();	// 敵の弾を削除
	m_pEnemyBullets.clear();
}
/*
*	@brief	初期化
*	@details	弾管理クラスの初期化
*	@param	pPlayer		プレイヤー
*	@param	pEnemies	敵管理
*	@return	なし
*/
void BulletManager::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// プレイヤーのポインターを取得
	m_pEnemyManager = pEnemies;// 敵全体のポインターを取得
	m_playerBulletCount = BulletParameters::MAX_PLAYER_BULLET_COUNT;// プレイヤーの弾の数を設定
}

/*
*	@brief	更新
*	@details	弾管理クラスの更新処理
*	@param	elapsedTime	経過時間
*	@return	なし
*/
void BulletManager::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;// 経過時間を更新
	UpdatePlayerBullets(elapsedTime);// プレイヤーの弾を更新
	for (auto& enemy : m_pEnemyManager->GetEnemies())UpdateEnemyBullets(elapsedTime, enemy);	// 敵の弾を更新
	CheckCollisionWithBullets();	// 弾同士の当たり判定

}

/*
*	@brief	描画
*	@details 弾管理クラスの描画処理
*	@param	なし
*	@return	なし
*/
void BulletManager::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto camera = m_pPlayer->GetCamera();// カメラを取得
	Matrix view = camera->GetViewMatrix();// ビュー行列
	Matrix proj = camera->GetProjectionMatrix();// プロジェクション行列
	for (const auto& bullet : m_pPlayerBullets)// プレイヤー弾の描画
	{
		bullet->RenderShadow(view, proj);// 影の描画
		bullet->Render(view, proj);// 弾の描画
	}
	for (const auto& bullet : m_pEnemyBullets)// 敵弾の描画
	{
		bullet->RenderShadow(view, proj);// 影の描画
		bullet->Render(view, proj);// 弾の描画
	}
}

/*
*	@brief	プレイヤーの弾を生成
*	@details	プレイヤーの弾を生成する
*	@param	position	生成位置
*	@param	direction	進行方向
*	@return	なし
*/
void BulletManager::CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{
	auto bullet = std::make_unique<PlayerBullet>();// プレイヤー弾の生成
	bullet->Initialize(m_pCommonResources);// 初期化
	bullet->MakeBall(position, direction);// 弾を生成
	m_pPlayerBullets.push_back(std::move(bullet));// プレイヤー弾を追加
	SetIsPlayerShoot(true);// 弾生成フラグを立てる
	m_pCommonResources->GetAudioManager()->PlaySound("Shoot", m_pPlayer->GetVolume());	// SEの再生
}

/*
*	@brief	敵の弾を生成
*	@details	敵の弾を生成する
*	@param	position	生成位置
*	@param	direction	進行方向
*	@return	なし
*/
void BulletManager::CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto camera = m_pPlayer->GetCamera();	//	カメラを取得
	Vector3 playerPos = camera->GetEyePosition();	// プレイヤーからカメラの情報を取得
	if (m_enemyBulletType == BulletType::SPECIAL)	// 弾が特殊弾なら
	{
		for (int i = 0; i < GetSpecialAttackCount(); i++)// 特殊攻撃の数だけ回転弾を生成
		{
			auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);// 敵弾の生成
			bullet->SetSEVolume(m_pPlayer->GetVolume());// SE音量を設定
			bullet->SetBulletType(m_enemyBulletType);// 弾の種類設定
			bullet->Initialize(m_pCommonResources);// 初期化
			bullet->SetShooter(m_pShooter);// 発射元を設定
			bullet->SetAngle(XM_2PI / GetSpecialAttackCount() * i);// 角度を設定
			bullet->SetDistance(0.0f);// 距離を設定
			bullet->SetIsExpand(true);// 展開フラグを立てる
			bullet->MakeBall(position, direction, playerPos);// 弾を生成
			m_pEnemyBullets.push_back(std::move(bullet));// 敵弾を追加
		}
	}
	else // 回転弾以外の弾の場合
	{
		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);// 敵弾の生成
		bullet->SetBulletType(m_enemyBulletType);// 弾の種類設定
		bullet->Initialize(m_pCommonResources);// 初期化
		bullet->MakeBall(position, direction, playerPos);// 弾を生成
		bullet->SetShooter(m_pShooter);// 発射元を設定
		m_pEnemyBullets.push_back(std::move(bullet));// 敵弾を追加
	}
}

/*
*	@brief	プレイヤーの弾を補充
*	@details	プレイヤーの弾を補充する
*	@param	なし
*	@return	なし
*/
void BulletManager::ReLoadPlayerBullet()
{
	m_reloadTimer += m_elapsedTime;// リロードタイマーを更新
	if (m_playerBulletCount < BulletParameters::MAX_PLAYER_BULLET_COUNT &&
		m_reloadTimer >= BulletParameters::RELOAD_INTERVAL)m_playerBulletCount++;// プレイヤーの弾を補充
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)m_reloadTimer = 0.0f;// リロードタイマーがリロード間隔を超えたらリセット
	if (m_playerBulletCount == BulletParameters::MAX_PLAYER_BULLET_COUNT)m_isReloading = false;// リロードが完了したらリロード中フラグを下げる
}

/*
*	@brief	プレイヤーの弾を消費
*	@details	プレイヤーの弾を消費する
*	@param	なし
*	@return	なし
*/
void BulletManager::ConsumePlayerBullet()
{
	if (m_playerBulletCount > 0)m_playerBulletCount--;	// プレイヤーの弾を消費
}

/*
*	@brief	追加ダメージを設定
*	@details	プレイヤーの弾に追加ダメージを設定する
*	@param	additionalDamage	追加ダメージ
*	@return	なし
*/
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	for (auto& bullet : m_pPlayerBullets)bullet->SetAdditionalDamage(additionalDamage);// 追加ダメージを設定
}

/*
*	@brief	プレイヤーの弾を更新
*	@details	プレイヤーの弾を更新する
*	@param	elapsedTime	経過時間
*	@return	なし
*/
void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	auto camera = m_pPlayer->GetCamera();// カメラを取得
	for (auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end();)
	{
		auto& bullet = *it;// 弾を取得
		bullet->SetCameraEye(camera->GetEyePosition());// カメラの位置
		bullet->SetCameraTarget(camera->GetTargetPosition());// カメラの注視点
		bullet->SetCameraUp(camera->GetUpVector());// カメラの上方向
		bullet->Update(elapsedTime);// 弾を更新
		if (CheckCollisionWithEnemies(bullet) ||// 敵との当たり判定
			bullet->IsExpired() ||// 寿命を迎えたら
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// 地面に着いたら
		{
			it = m_pPlayerBullets.erase(it);// 削除
		}
		else
		{
			++it;// 次の要素へ
		}
	}
}

/*
*	@brief	敵の弾を更新
*	@details	敵の弾を更新する
*	@param	elapsedTime	経過時間
*	@param	enemy		敵
*	@return	なし
*/
void BulletManager::UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy)
{
	auto camera = enemy->GetCamera();// カメラを取得
	for (auto it = m_pEnemyBullets.begin(); it != m_pEnemyBullets.end();)
	{
		auto& bullet = *it;// 弾を取得
		IEnemy* shooter = bullet->GetShooter();// 発射元を取得
		if (!shooter) { it = m_pEnemyBullets.erase(it); continue; }	// 発射元が存在しない場合は削除
		if (shooter != enemy.get()) { ++it; continue; }	// 発射元が自分でない場合は次の要素へ
		bullet->SetCameraEye(camera->GetEyePosition());// カメラの位置
		bullet->SetCameraTarget(camera->GetTargetPosition());// カメラの注視点
		bullet->SetCameraUp(camera->GetUpVector());// カメラの上方向
		bullet->SetEnemyPosition(shooter->GetPosition());// 敵の位置
		bullet->SetCurrentTarget(m_pPlayer->GetPlayerPos());// プレイヤーの位置
		bullet->Update(elapsedTime);// 弾を更新
		if (CheckCollisionWithPlayer(bullet, enemy) ||// プレイヤーとの当たり判定
			bullet->IsExpired() ||// 寿命を迎えたら
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// 地面に着いたら
		{
			it = m_pEnemyBullets.erase(it);// 削除
		}
		else ++it;// 次の要素へ
	}
}



/*
*	@brief	プレイヤーの弾が敵に当たったかどうか
*	@details	プレイヤーの弾と敵の当たり判定を行う
*	@param	bullet	チェック対象のプレイヤーの弾
*	@return	当たればtrue、当たらなければfalse
*/
bool BulletManager::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
{
	for (auto& enemy : m_pEnemyManager->GetEnemies())	// 敵との当たり判定
	{
		// 敵とプレイヤーの弾の当たり判定
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			if (enemy->GetCanAttack() == true)// 攻撃可能な敵の場合
				enemy->ApplyDamageToEnemy(bullet->Damage());// 敵のHPを減らす
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;// エフェクトを追加　ボスの場合は大きめにする
			m_pEnemyManager->GetEffects().push_back(std::make_unique<Effect>(// エフェクトを追加
				m_pCommonResources,// 共通リソース
				Effect::EffectType::ENEMY_HIT,// エフェクトの種類
				enemy->GetPosition(),// 位置
				effectSize,// サイズ
				enemy->GetMatrix()));// 行列
			enemy->SetEnemyHitByPlayerBullet(true);// プレイヤーの弾が敵に当たったらヒットフラグを立てる
			m_pCommonResources->GetAudioManager()->PlaySound("Hit", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);// SEを再生
			return true;// 当たったらtrueを返す
		}
	}
	return false;// 当たらなかったらfalseを返す
}


/*
*	@brief	プレイヤーと敵の弾の当たり判定
*	@details	プレイヤーと敵の弾の当たり判定を行う
*	@param	bullet	チェック対象の敵の弾
*	@param	enemy	敵
*	@return	当たればtrue、当たらなければfalse
*/
bool BulletManager::CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy)
{
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());// プレイヤーとの当たり判定
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))// プレイヤーと敵の弾の当たり判定
	{
		enemy->SetPlayerHitByEnemyBullet(true);// プレイヤーが敵の弾に当たったフラグを立てる
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetDirection());// プレイヤーの弾の方向を取得
		enemy->GetPlayer()->SetisPlayEffect(true);// プレイヤーがエフェクトを再生するフラグを立てる
		enemy->GetPlayer()->SetisPlayerDamage(true);// プレイヤーがダメージを受けたフラグを立てる
		return true; // 衝突したらtrueを返す
	}
	return false; // 衝突しなければfalseを返す}
}

/*
*	@brief	弾同士の当たり判定
*	@details	敵の弾とプレイヤーの弾の当たり判定を行う
*	@param	なし
*	@return	なし
*/
void BulletManager::CheckCollisionWithBullets()
{
	// 削除対象の弾を記録する「セット」(ポインターだけあればよく、値は不要なためunordered_setを使用)
	std::unordered_set<PlayerBullet*> playerBulletsToRemove;// プレイヤーの弾
	std::unordered_set<EnemyBullet*> enemyBulletsToRemove;// 敵の弾
	for (auto& playerBullet : m_pPlayerBullets)	// プレイヤーの弾と敵の弾の衝突判定(総当たり)
	{
		for (auto& enemyBullet : m_pEnemyBullets)
		{
			if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))	// 衝突があった場合、削除対象に追加
			{
				m_pCommonResources->GetAudioManager()->PlaySound("BulletCollision", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);// 弾同士の衝突音を再生
				enemyBulletsToRemove.insert(enemyBullet.get());	// 敵の弾を消す
				playerBulletsToRemove.insert(playerBullet.get());// プレイヤーの弾を消す
			}
		}
	}
	m_pEnemyBullets.erase(	// まとめて消す(敵の弾)
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),
			[&enemyBulletsToRemove](const std::unique_ptr<EnemyBullet>& bullet)
			{
				return enemyBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pEnemyBullets.end()
	);
	m_pPlayerBullets.erase(	// まとめて消す(プレイヤーの弾)
		std::remove_if(m_pPlayerBullets.begin(), m_pPlayerBullets.end(),
			[&playerBulletsToRemove](const std::unique_ptr<PlayerBullet>& bullet)
			{
				return playerBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pPlayerBullets.end()
	);
}

/*
*	@brief	指定した敵の弾をすべて削除（敵が死亡したときなど）
*	@details	指定した敵が発射した弾をすべて削除する
*	@param	shooter	削除対象の敵
*	@return	なし
*/
void BulletManager::RemoveBulletsByShooter(IEnemy* shooter)
{
	if (!shooter || m_pEnemyBullets.empty())	return;// 発射元が nullptr または無効なら何もしない
	m_pEnemyBullets.erase(	// 発射元が一致する弾をすべて削除
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),// 削除対象を探す
			[shooter](const std::unique_ptr<EnemyBullet>& bullet)
			{
				IEnemy* bulletShooter = bullet->GetShooter();// 弾を撃った敵を取得
				return (bulletShooter == shooter || bulletShooter == nullptr);// 発射元が一致するか、無効なら削除対象
			}),
		m_pEnemyBullets.end()
	);
}
