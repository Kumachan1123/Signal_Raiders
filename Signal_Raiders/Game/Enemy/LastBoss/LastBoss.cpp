/*
*	@file LastBoss.cpp
*	@brief ラスボスクラス
*/
#include <pch.h>
#include "LastBoss.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"



/*
*	@brief コンストラクタ
*	@param[in] pPlayer プレイヤーのポインター
*	@param[in] resources 共通リソース
*	@param[in] hp HP
*	@return なし
*/
LastBoss::LastBoss(Player* pPlayer, CommonResources* resources, int hp)
	: m_commonResources(resources)// 共通リソース
	, m_currentHP(hp)// 現在のHP
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// 攻撃のクールダウンタイム
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// 特殊攻撃のクールダウンタイム
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// 特殊攻撃のクールダウンタイム(初期化用)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// 弾のクールダウンタイム
	, m_time(0.0f)// 時間
	, m_SEVolume(0.0f)// SEの音量
	, m_SEVolumeCorrection(0.0f)// SEの音量補正
	, m_pBossAI(nullptr)// AI
	, m_pBossModel(nullptr)// モデル
	, m_pHPBar(nullptr)// HPバー
	, m_pPlayer(pPlayer)// プレイヤーのポインター
	, m_pCamera(pPlayer->GetCamera())// カメラのポインター
	, m_bossBS()// ボスの境界球
	, m_enemyBulletBS()// 敵の弾の境界球
	, m_playerBS()// プレイヤーの境界球
	, m_position(0.0f, 0.0f, 0.0f)// 座標
	, m_velocity(0.0f, 0.0f, 0.0f)// 速度
	, m_rotate(0.0f, 0.0f, 0.0f)// 回転
	, m_isDead(false)// 死亡フラグ
	, m_isHitToPlayer(false)// プレイヤーに当たったか
	, m_isHitToOtherEnemy(false)// 他の敵に当たったか
	, m_isEnemyHitByPlayerBullet(false)// 敵がプレイヤーの弾に当たったか
	, m_isPlayerHitByEnemyBullet(false)// プレイヤーが敵の弾に当たったか
	, m_canAttack(true)// 攻撃可能か
	, m_isAttack(false)// 攻撃中か
	, m_bossBulletType(BossBulletType::NORMAL)// ボスの弾の種類
	, m_bulletType(EnemyBullet::BulletType::NORMAL)// 弾の種類
	, m_audioManager{ }// オーディオマネージャー
{
}

/*
*	@brief デストラクタ
*	@return なし
*/
LastBoss::~LastBoss() { m_pBulletManager->RemoveBulletsByShooter(this); }// 弾を削除
/*
*	@brief 初期化
*	@return なし
*/
void LastBoss::Initialize()
{
	DrawCollision::Initialize(m_commonResources);// 衝突描画初期化
	m_pBossModel = std::make_unique<LastBossModel>();// モデルの初期化
	m_pBossModel->Initialize(m_commonResources);// モデルの初期化
	m_maxHP = m_currentHP;// 最大HPの初期化
	m_pHPBar = std::make_unique<EnemyHPBar>();// HPバーの生成
	m_pHPBar->SetEnemyHP(m_currentHP);// HPバーのHPを設定
	m_pHPBar->Initialize(m_commonResources);// HPバーの初期化
	m_pBossAI = std::make_unique<BossAI>(this);// AIの初期化
	m_pBossAI->Initialize();// AIの初期化	
	m_pBossSheild = std::make_unique<BossSheild>(m_maxHP, this);// シールドの初期化
	m_pBossSheild->Initialize(m_commonResources);// シールドの初期化
	m_position = EnemyParameters::INITIAL_LASTBOSS_POSITION;// 初期位置
	m_pBossAI->SetPosition(m_position);// AIに位置を設定
	m_bossBS.Center = m_position;// ボスの境界球の中心を設定
	m_bossBS.Radius = EnemyParameters::NORMAL_BOSS_RADIUS;// ボスの境界球の半径を設定
	m_SEVolume = m_pPlayer->GetVolume();// SEの音量を設定
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();// SEの音量補正を設定
}
/*
*	@brief 更新
*	@param[in] elapsedTime フレーム時間
*	@return なし
*/
void LastBoss::Update(float elapsedTime)
{
	m_cameraEye = m_pCamera->GetEyePosition();// カメラの位置を取得
	m_cameraTarget = m_pCamera->GetTargetPosition();// カメラの注視点を取得
	m_cameraUp = m_pCamera->GetUpVector();// カメラの上方向を取得
	m_pBossModel->SetState(m_pBossAI->GetState());// モデルのアニメーション更新
	m_pBossAI->Update(elapsedTime);// AIの更新
	m_position = m_pBossAI->GetPosition();// 敵の座標を更新
	m_audioManager->Update();// オーディオマネージャーの更新
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// 攻撃のクールダウンタイムを取得
	m_specialAttackCooldown -= elapsedTime;// 特殊攻撃のクールダウンタイムを減らす
	this->ShootBullet();// 弾発射
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// 境界球の中心座標を更新
	this->BulletPositioning();// 弾の位置設定
	m_pHPBar->SetCurrentHP(m_currentHP);// HPバーのHPを更新
	m_pHPBar->Update(elapsedTime);// HPバーの更新
	if (m_currentHP <= m_maxHP / 2)m_pBossSheild->SetSheild(true);// HPが半分以下になったらシールドを出す
	m_pBossSheild->Update(elapsedTime);// シールドの更新
	m_isDead = m_pHPBar->GetIsDead();// 死んだかどうかを受け取る
}
/*
*	@brief	描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void LastBoss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキスト
	auto states = m_commonResources->GetCommonStates();// ステート
	// ワールド行列を設定
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale())// スケール
		* Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())// 回転
		* Matrix::CreateTranslation(m_position);// 位置
	// シールドのワールド行列を設定
	Matrix sheildWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 3)// スケール(シールドの大きさ)
		* Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())// 回転
		* Matrix::CreateTranslation(m_position);// 位置
	m_pBossSheild->SetPosition(m_bossBS.Center);// シールドの座標を設定
	m_pBossSheild->SetRotation(m_pBossAI->GetRotation());// シールドの回転を設定
	m_pBossModel->Render(context, states, enemyWorld, view, proj);// モデル描画
	m_pBossSheild->Render(context, states, sheildWorld, view, proj);// シールド描画
	Vector3 hpBarPos = m_position - EnemyParameters::BOSS_HPBAR_OFFSET;// HPバーの位置を設定
	m_pHPBar->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));// HPバーのスケールを設定
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);// HPバー描画

}
/*
*	@brief	当たり判定描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void LastBoss::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(proj);// 未使用の警告を出さない
#ifdef _DEBUG
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 衝突判定の描画
	DrawCollision::DrawStart(view, proj);// 描画開始
	DirectX::XMVECTOR color = Colors::Black;// 色
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// 当たった
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// 当たっていない
	DrawCollision::DrawBoundingSphere(m_bossBS, color);// 境界球の描画
	DrawCollision::DrawEnd();// 描画終了
#endif
}
/*
*	@brief	シールド展開音再生
*	@return	なし
*/
void LastBoss::PlayBarrierSE()
{
	m_audioManager->PlaySound("Barrier", this->GetSheildSEVolume());// サウンド再生 
}
/*
*	@brief	弾発射
*	@return	なし
*/
void LastBoss::ShootBullet()
{
	using namespace DirectX::SimpleMath;
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)	// 攻撃のクールダウンタイムを管理
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());// クォータニオンから方向ベクトルを計算
		this->CreateBullet();// 弾を発射
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);// クールダウンタイムを設定
	}
	if (m_specialAttackCooldown <= 0.0f)// 特殊攻撃のクールダウンタイムを管理
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		m_pBulletManager->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE * 2);// 弾のサイズを設定
		m_pBulletManager->SetShooter(this);// 弾を発射したオブジェクトを設定
		this->CreateSpiralBullet();// 		// 特殊攻撃
		m_specialAttackCooldown = m_initSpecialAttackCooldown;// クールダウンタイムを設定
	}
}
/*
*	@brief	弾の位置設定
*	@return	なし
*/
void LastBoss::BulletPositioning()
{
	using namespace DirectX::SimpleMath;

	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())// 弾の発射位置を設定
		* Matrix::CreateTranslation(m_position);
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);// 中央の座標に回転を適用
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);// 左の座標に回転を適用
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);// 右の座標に回転を適用

}
/*
*	@brief	弾の生成
*	@return	なし
*/
void LastBoss::CreateBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	float angleOffset = XMConvertToRadians(EnemyParameters::BOSS_BULLET_ANGLE); // 30度の角度オフセット
	m_pBulletManager->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE);// 弾のサイズを設定
	m_pBulletManager->SetShooter(this);// 弾を発射したオブジェクトを設定
	switch (GetBulletType())	// Enemiesクラスで設定した弾のタイプによって処理を分岐
	{
	case BossBulletType::NORMAL:// 通常弾
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// 中央の弾を発射
		break;
	case BossBulletType::TWIN:// 二発

		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::NORMAL);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::NORMAL);// 右の弾を発射
		break;
	case BossBulletType::THREE:// 三発
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// 中央の弾を発射
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::NORMAL);// 左の弾を発射
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::NORMAL);// 右の弾を発射
		break;
	}
}
/*
*	@brief	中央から弾を発射
*	@param[in] type 弾の種類
*/
void LastBoss::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_pBulletManager->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// 弾を生成
}
/*
*	@brief	左の弾を発射
*	@param[in] angleOffset 角度オフセット
*	@param[in] type 弾の種類
*/
void LastBoss::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX::SimpleMath;
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);// 左方向
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_pBulletManager->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosLeft, leftDirection);// 弾を生成
}
/*
*	@brief 右の弾を発射
*	@param[in] angleOffset 角度オフセット
*	@param[in] type 弾の種類
*/
void LastBoss::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	using namespace DirectX::SimpleMath;
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);// 右方向
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	m_pBulletManager->SetEnemyBulletType(type);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosRight, rightDirection);// 弾を生成
}
/*
*	@brief	真下に落ちて加速する弾を発射
*	@return	なし
*/
void LastBoss::CreateVerticalBullet()
{
	m_pBulletManager->SetEnemyBulletType(EnemyBullet::BulletType::SPEED);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// 弾を生成
}
/*
*	@brief	特殊攻撃をする
*	@return	なし
*/
void LastBoss::CreateSpiralBullet()
{
	m_pBulletManager->SetEnemyBulletType(EnemyBullet::BulletType::SPECIAL);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// 弾を生成
}
/*
*	@brief	敵のHPを設定
*	@param[in] hp 敵のHP
*	@return	なし
*/
void LastBoss::SetEnemyHP(int hp)
{
	if (m_pBossSheild->GetSheildHP() > 0 &&// シールドがある場合
		m_pBossSheild->GetSheild() == true)// シールドが展開されている場合
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);// シールドのHPを減らす
		if (m_pBossSheild->GetSheildHP() <= 0)m_audioManager->PlaySound("BarrierBreak", m_pPlayer->GetVolume());// シールドが壊れたらサウンド再生 
	}
	else m_currentHP -= hp;// シールドがない場合は敵のHPを減らす
}