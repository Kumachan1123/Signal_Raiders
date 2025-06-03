/*
*	@file	BossBase.cpp
*	@brief	ボスベースクラス
*/
#include <pch.h>
#include "BossBase.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"


/*
*	@brief	コンストラクタ
*	@details ボスベースクラスのコンストラクタ
*	@param pPlayer		プレイヤーのポインタ
*	@param resources	共通リソース
*	@param hp			ボスの体力
*	@return	なし
*/
BossBase::BossBase(Player* pPlayer, CommonResources* resources, int hp)
	: m_pPlayer(pPlayer)// プレイヤーのポインタを設定
	, m_pCamera(pPlayer->GetCamera())// カメラのポインタを設定
	, m_bossBS{}// 境界球の初期化
	, m_pCommonResources(resources)// 共通リソース
	, m_currentHP(hp)// 現在のHP
	, m_pBulletManager(nullptr)// 弾管理クラスのポインタを設定
	, m_maxHP{}// 最大HP
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// 攻撃のクールダウンタイム
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// 特殊攻撃のクールダウンタイム
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// 特殊攻撃のクールダウンタイム(初期化用)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// 弾のクールダウンタイム
	, m_bulletSize(EnemyParameters::BOSS_BULLET_SIZE)// 弾のサイズ
	, m_bossDamage(EnemyParameters::BOSS_DAMAGE)// ボスがプレイヤーに与えるダメージ
	, m_SEVolume(0.0f)// SEの音量
	, m_SEVolumeCorrection(0.0f)// SEの音量補正
	, m_isBarrierSEPlay(false)// バリアSEの再生フラグ
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
*	@details 倒されたら弾を削除する
*	@param	なし
*	@return	なし
*/
BossBase::~BossBase() { m_pBulletManager->RemoveBulletsByShooter(this); }// 弾を削除
/*
*	@brief	初期化処理
*	@details ボスベースクラスの初期化
*	@param	なし
*	@return	なし
*/
void BossBase::Initialize()
{
	// ボス生成
	m_pBoss = EnemyFactory::CreateBoss(m_bossType, this, m_pCommonResources);
	// 当たり判定描画の初期化
	DrawCollision::Initialize(m_pCommonResources);
	// 最大HPを設定
	m_maxHP = m_currentHP;
	// HPバー生成
	m_pHPBar = std::make_unique<EnemyHPBar>();
	// HPバーのHPを設定
	m_pHPBar->SetEnemyMaxHP(m_currentHP);
	// HPバー初期化
	m_pHPBar->Initialize(m_pCommonResources);
	// ボス初期化
	m_pBoss->Initialize();
	// ボスAI生成
	m_pBossAI = std::make_unique<BossAI>(this);
	// ボスAI初期化
	m_pBossAI->Initialize();
	// AIに座標を設定
	m_pBossAI->SetPosition(m_position);
	// 境界球の中心座標を設定
	m_bossBS.Center = m_position;
	// SEの音量を設定
	m_SEVolume = m_pPlayer->GetVolume();
	// SEの音量補正を設定
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();
	// シールド生成
	m_pBossSheild = std::make_unique<BossSheild>();
	// シールドの初期化
	m_pBossSheild->SetUp(m_maxHP, this);
	// シールド初期化
	m_pBossSheild->Initialize(m_pCommonResources);

}
/*
*	@brief	更新処理
*	@details ボスベースクラスの更新処理
*	@param elapsedTime 経過時間
*	@return	なし
*/
void BossBase::Update(float elapsedTime)
{
	// カメラの位置を取得
	m_cameraEye = m_pCamera->GetEyePosition();
	// カメラの注視点を取得
	m_cameraTarget = m_pCamera->GetTargetPosition();
	// カメラの上方向を取得
	m_cameraUp = m_pCamera->GetUpVector();
	// ボスの更新
	m_pBoss->ChangeState();
	// AIの更新
	m_pBossAI->Update(elapsedTime);
	// 敵の座標を更新
	m_position = m_pBossAI->GetPosition();
	// オーディオマネージャーの更新
	m_pCommonResources->GetAudioManager()->Update();
	// 攻撃のクールダウンタイムを取得
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	// 特殊攻撃のクールダウンタイムを減らす
	m_specialAttackCooldown -= elapsedTime;
	// 弾の位置設定
	m_pBoss->BulletPositioning();
	// 弾発射
	ShootBullet();
	// 境界球の中心座標を更新
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;
	// 通常時のボスの当たり判定を設定
	m_bossBS.Radius = m_defaultHitRadius;
	// HPバーのHPを更新
	m_pHPBar->SetCurrentHP(m_currentHP);
	// HPバーの更新
	m_pHPBar->Update(elapsedTime);
	// HPが半分以下になったらシールドを出す
	if (m_currentHP <= m_maxHP / 2)	m_pBossSheild->SetSheild(true);
	// シールドの更新
	m_pBossSheild->Update(elapsedTime);
	// 死んだかどうかを受け取る
	m_isDead = m_pHPBar->GetIsDead();
}
/*
*	@brief	描画処理
*	@details ボスベースクラスの描画処理
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return	なし
*/
void BossBase::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 敵の座標を更新
	m_position = m_pBossAI->GetPosition();
	// 敵の回転を更新
	m_quaternion = m_pBossAI->GetRotation();
	// 敵のスケールを更新
	m_scale = m_pBossAI->GetScale();
	// ボス描画
	m_pBoss->Draw(view, proj);
}
/*
*	@brief	当たり判定描画処理
*	@details ボスベースクラスの当たり判定描画処理（デバッグ用）
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return	なし
*/
void BossBase::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(view);
	// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// 描画開始
	DrawCollision::DrawStart(view, proj);
	// 色を設定
	DirectX::XMVECTOR color = Colors::Black;
	// プレイヤーに当たっている場合
	if (m_isHitToPlayer)
	{
		// 他の敵にも当たっているならトマト色、そうでなければ青
		color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	}
	// プレイヤーに当たっていない場合
	else
	{
		// 他の敵に当たっているなら白、そうでなければ黒
		color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	}
	// 境界球の描画
	DrawCollision::DrawBoundingSphere(m_bossBS, color);
	// 描画終了
	DrawCollision::DrawEnd();
#endif
}
/*
*	@brief	シールド展開音再生
*	@details シールド展開音を再生する
*	@param	なし
*	@return	なし
*/
void BossBase::PlayBarrierSE()
{
	// 既に再生中なら何もしない
	if (m_isBarrierSEPlay) return;
	// シールド展開音再生 
	m_pCommonResources->GetAudioManager()->PlaySound("Barrier", m_pPlayer->GetVolume());
	// 再生フラグを立てる
	m_isBarrierSEPlay = true;
}
/*
*	@brief	弾発射
*	@details ボスが弾を発射する処理
*	@param	なし
*	@return	なし
*/
void BossBase::ShootBullet()
{
	using namespace DirectX::SimpleMath;
	// 通常攻撃のクールダウンタイムをチェック
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)
	{
		// 通常弾のサウンド再生
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
		// 弾の飛ぶ方向を、現在のボスの回転から計算
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());
		// 通常弾の生成
		m_pBoss->CreateBullet();
		// 攻撃クールダウンタイムをリセット
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);
	}
	// 特殊攻撃のクールダウンタイムをチェック
	if (m_specialAttackCooldown <= 0.0f)
	{
		// 特殊弾のサウンド再生
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
		// 弾のサイズを設定
		m_pBulletManager->SetEnemyBulletSize(m_bulletSize);
		// 発射したオブジェクトを指定
		m_pBulletManager->SetShooter(this);
		// 特殊攻撃（スパイラル弾）の生成
		CreateSpiralBullet();
		// 特殊攻撃のクールダウンをリセット
		m_specialAttackCooldown = m_initSpecialAttackCooldown;
	}
}

/*
*	@brief	真下に落ちて加速する弾を発射
*	@details ボスが真下に落ちて加速する弾を発射する処理
*	@param	なし
*	@return	なし
*/
void BossBase::CreateVerticalBullet()
{
	// 弾の種類を設定
	m_pBulletManager->SetEnemyBulletType(BulletType::SPEED);
	// 弾を生成
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);
}

/*
*	@brief	特殊攻撃をする
*	@details ボスが特殊攻撃をする処理
*	@param	なし
*	@return	なし
*/
void BossBase::CreateSpiralBullet()
{
	// 弾の種類を設定
	m_pBulletManager->SetEnemyBulletType(BulletType::SPECIAL);
	// 弾を生成
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);
}
/*
*	@brief	敵のHPを設定
*	@details 敵のHPを設定する
*	@param hp 敵のHP
*	@return	なし
*/
void BossBase::ApplyDamageToEnemy(int hp)
{
	// シールドがあり、シールドが展開されている場合
	if (m_pBossSheild->GetSheildHP() > 0 && m_pBossSheild->GetSheild() == true)
	{
		// シールドのHPを減らす
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);
		// シールドが壊れたらサウンド再生
		if (m_pBossSheild->GetSheildHP() <= 0)m_pCommonResources->GetAudioManager()->PlaySound("BarrierBreak", m_pPlayer->GetVolume());
	}
	else // シールドがない場合は敵のHPを減らす
		m_currentHP -= hp;
}