/*
*	@file	BulletManager.cpp
*	@brief	弾管理クラス
*/
#include <pch.h>
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
	// プレイヤーの弾を削除
	for (auto& bullet : m_pPlayerBullets)bullet.reset();
	// プレイヤーの弾をクリア
	m_pPlayerBullets.clear();
	// 敵の弾を削除
	for (auto& bullet : m_pEnemyBullets)bullet.reset();
	// 敵の弾をクリア
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
	// プレイヤーのポインターを取得
	m_pPlayer = pPlayer;
	// 敵全体のポインターを取得
	m_pEnemyManager = pEnemies;
	// プレイヤーの弾の数を設定
	m_playerBulletCount = BulletParameters::MAX_PLAYER_BULLET_COUNT;
}

/*
*	@brief	更新
*	@details	弾管理クラスの更新処理
*	@param	elapsedTime	経過時間
*	@return	なし
*/
void BulletManager::Update(float elapsedTime)
{
	// 経過時間を更新
	m_elapsedTime = elapsedTime;
	// プレイヤーの弾を更新
	UpdatePlayerBullets(elapsedTime);
	// 敵の弾を更新
	for (auto& enemy : m_pEnemyManager->GetEnemies())UpdateEnemyBullets(elapsedTime, enemy);
	// 弾同士の当たり判定
	CheckCollisionWithBullets();

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
	// カメラを取得
	auto camera = m_pPlayer->GetCamera();
	// ビュー行列
	Matrix view = camera->GetViewMatrix();
	// プロジェクション行列
	Matrix proj = camera->GetProjectionMatrix();
	// プレイヤー弾の描画
	for (const auto& bullet : m_pPlayerBullets)
	{
		// 影の描画
		bullet->RenderShadow(view, proj);
		// 弾の描画
		bullet->Render(view, proj);
	}
	// 敵弾の描画
	for (const auto& bullet : m_pEnemyBullets)
	{
		// 影の描画
		bullet->RenderShadow(view, proj);
		// 弾の描画
		bullet->Render(view, proj);
	}
}

/*
*	@brief	プレイヤーの弾を生成
*	@details	プレイヤーの弾を生成する
*	@param	position	生成位置
*	@param	direction	進行方向
*	@return	なし
*/
void BulletManager::CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction)
{
	// プレイヤー弾の生成
	auto bullet = std::make_unique<PlayerBullet>();
	// 初期化
	bullet->Initialize(m_pCommonResources);
	// 弾を生成
	bullet->MakeBall(position, direction);
	// プレイヤー弾を追加
	m_pPlayerBullets.push_back(std::move(bullet));
	// 弾生成フラグを立てる
	SetIsPlayerShoot(true);
	// SEの再生
	m_pCommonResources->GetAudioManager()->PlaySound("Shoot", m_pPlayer->GetVolume());
}

/*
*	@brief	敵の弾を生成
*	@details	敵の弾を生成する
*	@param	position	生成位置
*	@param	direction	進行方向
*	@return	なし
*/
void BulletManager::CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	カメラを取得
	auto camera = m_pPlayer->GetCamera();
	// プレイヤーからカメラの情報を取得
	Vector3 playerPos = camera->GetEyePosition();
	// 弾が特殊弾なら
	if (m_enemyBulletType == BulletType::SPECIAL)
	{
		// 特殊攻撃の数だけ回転弾を生成
		for (int i = 0; i < GetSpecialAttackCount(); i++)
		{
			// 敵弾の生成
			auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);
			// SE音量を設定
			bullet->SetSEVolume(m_pPlayer->GetVolume());
			// 弾の種類設定
			bullet->SetBulletType(m_enemyBulletType);
			// 初期化
			bullet->Initialize(m_pCommonResources);
			// 発射元を設定
			bullet->SetShooter(m_pShooter);
			// 角度を設定
			bullet->SetAngle(XM_2PI / GetSpecialAttackCount() * i);
			// 距離を設定
			bullet->SetDistance(0.0f);
			// 展開フラグを立てる
			bullet->SetIsExpand(true);
			// 弾を生成
			bullet->MakeBall(position, direction, playerPos);
			// 敵弾を追加
			m_pEnemyBullets.push_back(std::move(bullet));
		}
	}
	else // 回転弾以外の弾の場合
	{
		// 敵弾の生成
		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize);
		// 弾の種類設定
		bullet->SetBulletType(m_enemyBulletType);
		// 初期化
		bullet->Initialize(m_pCommonResources);
		// 弾を生成
		bullet->MakeBall(position, direction, playerPos);
		// 発射元を設定
		bullet->SetShooter(m_pShooter);
		// 敵弾を追加
		m_pEnemyBullets.push_back(std::move(bullet));
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
	// リロードタイマーを更新
	m_reloadTimer += m_elapsedTime;
	// プレイヤーの弾が最大数未満で、リロードタイマーがリロード間隔を超えたら
	if (m_playerBulletCount < BulletParameters::MAX_PLAYER_BULLET_COUNT && m_reloadTimer >= BulletParameters::RELOAD_INTERVAL)
	{
		// プレイヤーの弾を補充
		m_playerBulletCount++;
	}
	// リロードタイマーがリロード間隔を超えたら
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)
	{
		m_reloadTimer = 0.0f;// リロードタイマーをリセット
	}
	// リロードが完了したら
	if (m_playerBulletCount == BulletParameters::MAX_PLAYER_BULLET_COUNT)
	{
		// リロード中フラグを下げる
		m_isReloading = false;
	}
}

/*
*	@brief	プレイヤーの弾を消費
*	@details	プレイヤーの弾を消費する
*	@param	なし
*	@return	なし
*/
void BulletManager::ConsumePlayerBullet()
{
	// プレイヤーの弾が1つ以上ある場合
	if (m_playerBulletCount > 0)
	{
		// プレイヤーの弾を消費
		m_playerBulletCount--;
	}
}

/*
*	@brief	追加ダメージを設定
*	@details	プレイヤーの弾に追加ダメージを設定する
*	@param	additionalDamage	追加ダメージ
*	@return	なし
*/
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	// プレイヤーの弾に追加ダメージを設定
	for (auto& bullet : m_pPlayerBullets)
	{
		// 追加ダメージを設定
		bullet->SetAdditionalDamage(additionalDamage);
	}
}

/*
*	@brief	プレイヤーの弾を更新
*	@details	プレイヤーの弾を更新する
*	@param	elapsedTime	経過時間
*	@return	なし
*/
void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	// カメラを取得
	auto camera = m_pPlayer->GetCamera();
	// プレイヤーの弾を更新
	for (auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end();)
	{
		// 配列から弾を取得
		auto& bullet = *it;
		// カメラの位置を取得して弾に設定
		bullet->SetCameraEye(camera->GetEyePosition());
		// カメラの注視点を取得して弾に設定
		bullet->SetCameraTarget(camera->GetTargetPosition());
		// カメラの上方向を取得して弾に設定
		bullet->SetCameraUp(camera->GetUpVector());
		// 弾を更新
		bullet->Update(elapsedTime);
		if (CheckCollisionWithEnemies(bullet) ||// 敵との当たり判定でtrueか
			bullet->IsExpired() ||// 寿命を迎えたか
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// 地面に着いたら
		{
			// 弾を削除
			it = m_pPlayerBullets.erase(it);
		}
		else// 当たらなかった場合
		{
			// 次の要素へ
			++it;
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
	// カメラを取得
	auto camera = enemy->GetCamera();
	// 敵の弾を更新
	for (auto it = m_pEnemyBullets.begin(); it != m_pEnemyBullets.end();)
	{
		// 配列から弾を取得
		auto& bullet = *it;
		// 発射元を取得
		IEnemy* shooter = bullet->GetShooter();
		// 発射元が存在しない場合は削除
		if (!shooter) { it = m_pEnemyBullets.erase(it); continue; }
		// 発射元が自分でない場合は次の要素へ
		if (shooter != enemy.get()) { ++it; continue; }
		// カメラの位置を取得して弾に設定
		bullet->SetCameraEye(camera->GetEyePosition());
		// カメラの注視点を取得して弾に設定
		bullet->SetCameraTarget(camera->GetTargetPosition());
		// カメラの上方向を取得して弾に設定
		bullet->SetCameraUp(camera->GetUpVector());
		// 敵の位置を取得して弾に設定
		bullet->SetEnemyPosition(shooter->GetPosition());
		// 弾の方向を取得して弾に設定
		bullet->SetCurrentTarget(m_pPlayer->GetPlayerPos());
		// 弾を更新
		bullet->Update(elapsedTime);
		if (CheckCollisionWithPlayer(bullet, enemy) ||// プレイヤーとの当たり判定でtrueか
			bullet->IsExpired() ||// 寿命を迎えたか
			bullet->GetBulletPosition().y <= BulletParameters::DELETE_BULLET_POSITION)// 地面に着いたら
		{
			// 削除
			it = m_pEnemyBullets.erase(it);
		}
		// 当たらなかった場合は次の要素へ
		else ++it;
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
	// 敵との当たり判定
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// 敵とプレイヤーの弾の当たり判定
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			// 攻撃可能な敵の場合
			if (enemy->GetCanAttack() == true)
			{
				// 敵のHPを減らす
				enemy->ApplyDamageToEnemy(bullet->Damage());
			}
			// エフェクトを追加　ボスの場合は大きめにする
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;
			// エフェクトを追加
			m_pEnemyManager->GetEffects().push_back(std::make_unique<Effect>(
				m_pCommonResources,// 共通リソース
				Effect::EffectType::ENEMY_HIT,// エフェクトの種類
				enemy->GetPosition(),// 位置
				effectSize,// サイズ
				enemy->GetMatrix()));// 行列
			// プレイヤーの弾が敵に当たったらヒットフラグを立てる
			enemy->SetEnemyHitByPlayerBullet(true);
			// SEを再生
			m_pCommonResources->GetAudioManager()->PlaySound("Hit", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);
			// 当たったらtrueを返す
			return true;
		}
	}
	// 当たらなかったらfalseを返す
	return false;
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
	// 発射元が撃った弾に境界球を設定
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
	// プレイヤーと敵の弾の当たり判定
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))
	{
		// プレイヤーが敵の弾に当たったフラグを立てる
		enemy->SetPlayerHitByEnemyBullet(true);
		// プレイヤーの弾の方向を取得
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetDirection());
		// プレイヤーがエフェクトを再生するフラグを立てる
		enemy->GetPlayer()->SetisPlayEffect(true);
		// プレイヤーがダメージを受けたフラグを立てる
		enemy->GetPlayer()->SetisPlayerDamage(true);
		// 衝突したらtrueを返す
		return true;
	}
	// 衝突しなければfalseを返す
	return false;
}

/*
*	@brief	弾同士の当たり判定
*	@details	敵の弾とプレイヤーの弾の当たり判定を行う
*	@param	なし
*	@return	なし
*/
void BulletManager::CheckCollisionWithBullets()
{
	// 削除対象の弾を記録するセット
	std::unordered_set<PlayerBullet*> playerBulletsToRemove;// プレイヤーの弾
	std::unordered_set<EnemyBullet*> enemyBulletsToRemove;// 敵の弾
	// プレイヤーの弾と敵の弾の衝突判定
	for (auto& playerBullet : m_pPlayerBullets)
	{
		// 総当たりで処理
		for (auto& enemyBullet : m_pEnemyBullets)
		{
			// 衝突があった場合
			if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))
			{
				// 弾同士の衝突音を再生
				m_pCommonResources->GetAudioManager()->PlaySound("BulletCollision", m_pPlayer->GetVolume() * BulletParameters::HIT_VOLUME);
				// 敵の弾を消す
				enemyBulletsToRemove.insert(enemyBullet.get());
				// プレイヤーの弾を消す
				playerBulletsToRemove.insert(playerBullet.get());
			}
		}
	}
	// 敵の弾をまとめて消す
	// enemyBulletsToRemoveに削除対象の弾が入っているので、これを使って削除する
	// std::remove_ifを使って、削除対象の弾を見つけて削除
	// 弾の実体は std::unique_ptr なので、ポインターを使って比較する
	m_pEnemyBullets.erase(
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),// 削除対象を探す
			[&enemyBulletsToRemove](const std::unique_ptr<EnemyBullet>& bullet)// ラムダ式で条件を指定
			{
				// 削除対象の弾がセットに含まれているかチェック
				return enemyBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pEnemyBullets.end()// 削除範囲の終端
	);
	// プレイヤーの弾をまとめて消す
	// playerBulletsToRemoveに削除対象の弾が入っているので、これを使って削除する
	// std::remove_ifを使って、削除対象の弾を見つけて削除
	// 弾の実体は std::unique_ptr なので、ポインターを使って比較する
	m_pPlayerBullets.erase(
		std::remove_if(m_pPlayerBullets.begin(), m_pPlayerBullets.end(),// 削除対象を探す
			[&playerBulletsToRemove](const std::unique_ptr<PlayerBullet>& bullet)// ラムダ式で条件を指定
			{
				// 削除対象の弾がセットに含まれているかチェック
				return playerBulletsToRemove.count(bullet.get()) > 0;
			}),
		m_pPlayerBullets.end()// 削除範囲の終端
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
	// 発射元が nullptr または無効なら何もしない
	if (!shooter || m_pEnemyBullets.empty()) return;
	// 発射元が一致する弾をすべて削除
	m_pEnemyBullets.erase(
		std::remove_if(m_pEnemyBullets.begin(), m_pEnemyBullets.end(),// 削除対象を探す
			[shooter](const std::unique_ptr<EnemyBullet>& bullet)// ラムダ式で条件を指定
			{
				// 弾を撃った敵を取得
				IEnemy* bulletShooter = bullet->GetShooter();
				// 発射元が一致するか、無効なら削除対象
				return (bulletShooter == shooter || bulletShooter == nullptr);
			}),
		m_pEnemyBullets.end()// 削除範囲の終端
	);
}
