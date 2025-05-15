#include <pch.h>
#include "BossBase.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
/*
*	@brief	コンストラクタ
*	@param[in] pPlayer		プレイヤーのポインタ
*	@param[in] resources	共通リソース
*	@param[in] hp			ボスの体力
*	@return	なし
*/
BossBase::BossBase(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)// 親クラスのコンストラクタを呼び出す
	, m_pPlayer(pPlayer)// プレイヤーのポインタを設定
	, m_pCamera(pPlayer->GetCamera())// カメラのポインタを設定
	, m_bossBS{}// 境界球の初期化
	, m_commonResources(resources)// 共通リソース
	, m_currentHP(hp)// 現在のHP
	, m_maxHP{}// 最大HP
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// 攻撃のクールダウンタイム
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// 特殊攻撃のクールダウンタイム
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// 特殊攻撃のクールダウンタイム(初期化用)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// 弾のクールダウンタイム
	, m_bulletSize(EnemyParameters::BOSS_BULLET_SIZE)// 弾のサイズ
	, m_bossDamage(EnemyParameters::BOSS_DAMAGE)// ボスがプレイヤーに与えるダメージ
	, m_SEVolume(0.0f)// SEの音量
	, m_SEVolumeCorrection(0.0f)// SEの音量補正
	, m_pBossModel{}// ボスモデル
	, m_pBossAI{}// AI
	, m_pHPBar{}// HPバー
	, m_time(0.0f)// 時間
	, m_position{}// 位置
	, m_velocity{}// 速度
	, m_rotate{}// 回転
	, m_enemyBSToPlayerArea{}// 敵とPlayerとの一定範囲の当たり判定に使う
	, m_enemyBulletBS{}// 敵の弾の境界球
	, m_playerBS{}// プレイヤーの境界球
	, m_matrix{}// マトリクス
	, m_isDead(false)// 死亡フラグ
	, m_isHitToPlayer(false)// プレイヤーに当たったか
	, m_isHitToOtherEnemy(false)// 他の敵に当たったか
	, m_isEnemyHitByPlayerBullet(false)// 敵がプレイヤーの弾に当たったか
	, m_isPlayerHitByEnemyBullet(false)// プレイヤーが敵の弾に当たったか
	, m_canAttack(true)// 攻撃可能か
	, m_isAttack(false)// 攻撃中か
	, m_bossType(BossType::NORMAL_BOSS)// ボスの種類
	, m_bossBulletType(BossBulletType::STAGE_4)// ボスの弾の種類
	, m_bulletType(BulletType::NORMAL)// 弾の種類
	, m_defaultHitRadius(EnemyParameters::NORMAL_BOSS_RADIUS)// デフォルトの当たり判定半径
	, m_defensiveHitRadius(EnemyParameters::BOSS_SHIELD_RADIUS)// 弾の発射位置
	, m_barrierBreakSize(EnemyParameters::BOSS_BARRIERBREAK_SIZE)// バリア破壊パーティクルのサイズ
	, m_deadEffectSize(EnemyParameters::BOSS_DEADEFFECT_SCALE)// ボスの死亡エフェクトのサイズ
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
BossBase::~BossBase() { m_pBulletManager->RemoveBulletsByShooter(this); }// 弾を削除
/*
*	@brief	初期化処理
*	@return	なし
*/
void BossBase::Initialize()
{
	m_pBoss = EnemyFactory::CreateBoss(m_bossType, this, m_commonResources);// ボス生成
	DrawCollision::Initialize(m_commonResources);// 当たり判定描画の初期化
	m_maxHP = m_currentHP;// 最大HPを設定
	m_pHPBar = std::make_unique<EnemyHPBar>();	// HPバー生成
	m_pHPBar->SetEnemyMaxHP(m_currentHP);// HPバーのHPを設定
	m_pHPBar->Initialize(m_commonResources);// HPバー初期化
	m_pBoss->Initialize();// ボス初期化
	m_pBossAI = std::make_unique<BossAI>(this);// ボスAI生成
	m_pBossAI->Initialize();// ボスAI初期化
	m_pBossAI->SetPosition(m_position);// AIに座標を設定
	m_bossBS.Center = m_position;// 境界球の中心座標を設定
	m_SEVolume = m_pPlayer->GetVolume();// SEの音量を設定
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();// SEの音量補正を設定
	m_pBossSheild = std::make_unique<BossSheild>();// シールド生成 これはタイプによって分岐予定
	m_pBossSheild->SetUp(m_maxHP, this);// シールドの初期化
	m_pBossSheild->Initialize(m_commonResources);// シールド初期化 これはタイプによって分岐予定
}
/*
*	@brief	更新処理
*	@param[in] elapsedTime 経過時間
*	@return	なし
*/
void BossBase::Update(float elapsedTime)
{
	m_cameraEye = m_pCamera->GetEyePosition();// カメラの位置を取得
	m_cameraTarget = m_pCamera->GetTargetPosition();// カメラの注視点を取得
	m_cameraUp = m_pCamera->GetUpVector();// カメラの上方向を取得
	m_pBoss->ChangeState();// ボスの更新
	m_pBossAI->Update(elapsedTime);// AIの更新
	m_position = m_pBossAI->GetPosition();// 敵の座標を更新
	m_commonResources->GetAudioManager()->Update();// オーディオマネージャーの更新
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// 攻撃のクールダウンタイムを取得
	m_specialAttackCooldown -= elapsedTime;// 特殊攻撃のクールダウンタイムを減らす
	m_pBoss->BulletPositioning();// 弾の位置設定
	this->ShootBullet();// 弾発射
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// 境界球の中心座標を更新
	m_bossBS.Radius = m_defaultHitRadius;// 通常時のボスの当たり判定を設定
	m_pHPBar->SetCurrentHP(m_currentHP);// HPバーのHPを更新
	m_pHPBar->Update(elapsedTime);// HPバーの更新
	if (m_currentHP <= m_maxHP / 2)	m_pBossSheild->SetSheild(true);// HPが半分以下になったらシールドを出す
	m_pBossSheild->Update(elapsedTime);// シールドの更新
	m_isDead = m_pHPBar->GetIsDead();// 死んだかどうかを受け取る
}
/*
*	@brief	描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void BossBase::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_position = m_pBossAI->GetPosition();// 敵の座標を更新
	m_quaternion = m_pBossAI->GetRotation();// 敵の回転を更新
	m_scale = m_pBossAI->GetScale();// 敵のスケールを更新
	m_pBoss->Draw(view, proj);// ボス描画
}
/*
*	@brief	当たり判定描画処理
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void BossBase::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(proj);// 未使用の警告を出さない
#ifdef _DEBUG
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
void BossBase::PlayBarrierSE()
{
	m_commonResources->GetAudioManager()->PlaySound("Barrier", this->GetSheildSEVolume());// サウンド再生 
}
/*
*	@brief	弾発射
*	@return	なし
*/
void BossBase::ShootBullet()
{
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)	// 攻撃のクールダウンタイムを管理
	{
		m_commonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());// クォータニオンから方向ベクトルを計算
		m_pBoss->CreateBullet();// 弾を発射
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);// クールダウンタイムを設定
	}
	if (m_specialAttackCooldown <= 0.0f)// 特殊攻撃のクールダウンタイムを管理
	{
		m_commonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// サウンド再生 
		m_pBulletManager->SetEnemyBulletSize(m_bulletSize);// 弾のサイズを設定
		m_pBulletManager->SetShooter(this);// 弾を発射したオブジェクトを設定
		this->CreateSpiralBullet();// 		// 特殊攻撃
		m_specialAttackCooldown = m_initSpecialAttackCooldown;// クールダウンタイムを設定
	}
}

/*
*	@brief	真下に落ちて加速する弾を発射
*	@return	なし
*/
void BossBase::CreateVerticalBullet()
{
	m_pBulletManager->SetEnemyBulletType(BulletType::SPEED);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// 弾を生成
}

/*
*	@brief	特殊攻撃をする
*	@return	なし
*/
void BossBase::CreateSpiralBullet()
{
	m_pBulletManager->SetEnemyBulletType(BulletType::SPECIAL);// 弾の種類を設定
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// 弾を生成
}
/*
*	@brief	敵のHPを設定
*	@param[in] hp 敵のHP
*	@return	なし
*/
void BossBase::SetEnemyMaxHP(int hp)
{
	if (m_pBossSheild->GetSheildHP() > 0 &&// シールドがある場合
		m_pBossSheild->GetSheild() == true)// シールドが展開されている場合
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);// シールドのHPを減らす
		if (m_pBossSheild->GetSheildHP() <= 0)// シールドが壊れたらサウンド再生
			m_commonResources->GetAudioManager()->PlaySound("BarrierBreak", m_pPlayer->GetVolume());
	}
	else m_currentHP -= hp;// シールドがない場合は敵のHPを減らす
}