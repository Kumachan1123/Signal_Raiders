/*
*	@file	BossSheild.cpp
*	@brief	ボスシールドクラス
*/
#include <pch.h>
#include "BossSheild.h"

/*
*	@brief	コンストラクタ
*	@details ボスシールドクラスのコンストラクタ
*	@return	なし
*/
BossSheild::BossSheild()
	: m_pCommonResources{}// 共通リソース
	, m_isSheild(false)// シールド展開フラグ
	, m_isParticle(false)// パーティクル再生フラグ
	, m_sheildSize(DirectX::SimpleMath::Vector3::Zero)// シールドのサイズ
	, m_sheildPosition(DirectX::SimpleMath::Vector3::Zero)// シールドの座標
	, m_sheildHP(0)// シールドのHP
	, m_bossType(BossShieldType::BOSS)// ボスの種類
	, m_pBoss(nullptr)// ボスクラスのポインタ
	, m_pSheildModel(nullptr)// シールドモデル
{
}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@return	なし
*/
BossSheild::~BossSheild()
{
	// 共通リソースをnullptrに設定
	m_pCommonResources = nullptr;
	// シールドモデルをnullptrに設定
	m_pSheildModel = nullptr;
	// ボスクラスのポインタをnullptrに設定
	m_pBoss = nullptr;
	// パーティクルをnullptrに設定
	m_pParticle = nullptr;
}

/*
*	@brief	シールドの初期化
*	@param sheildHP シールドのHP
*	@param pBoss ボスクラスのポインタ
*	@return	なし
*/
void BossSheild::SetUp(int sheildHP, IEnemy* pBoss)
{
	using namespace DirectX::SimpleMath;
	// シールドのHPを設定
	m_sheildHP = sheildHP;
	// ボスクラスのポインタを設定
	m_pBoss = pBoss;
	// シールド展開フラグを立てる
	m_isSheild = false;
	// シールドのサイズを初期化
	m_sheildSize = Vector3::Zero;
	// ボスの位置をシールドの位置に設定
	m_sheildPosition = pBoss->GetPosition();
}
/*
*	@brief	初期化
*	@param resources 共通リソース
*	@return	なし
*/
void BossSheild::Initialize(CommonResources* resources)
{
	// 共通リソースを設定
	m_pCommonResources = resources;
	// パーティクルを生成
	m_pParticle = std::make_unique<Particle>(ParticleUtility::Type::BARRIERBREAK, 0.0f);
	// パーティクル初期化
	m_pParticle->Initialize(m_pCommonResources);
	// マネージャーからシールドモデルを取得
	m_pSheildModel = m_pCommonResources->GetModelManager()->GetModel("Barrier");
}

/*
*	@brief	更新
*	@param elapsedTime 経過時間
*	@return	なし
*/
void BossSheild::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ボスクラスのポインタを取得
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);
	// シールドが展開されている間
	if (m_isSheild)
	{
		// シールド展開音を再生する
		pBoss->PlayBarrierSE();
		// シールドのサイズを滑らかに拡大していく
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3(0.4f), EnemyParameters::BOSS_SHIELD_SCALE_SPEED);
		// ボスの境界球を防御モードのサイズに設定
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefensiveHitRadius();
		// 通常攻撃のクールダウンを短くして攻撃速度を上げる
		pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);
		// 特殊攻撃のクールダウンも短縮する
		pBoss->SetInitSpecialAttacCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN / 2);
	}
	// シールドが破壊されたら
	if (m_sheildHP <= 0)
	{
		// パーティクル再生フラグを立てる
		m_isParticle = true;
		// シールド無効化
		m_isSheild = false;
		// 境界球のサイズを通常時の大きさに戻す
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefaultHitRadius();
		// パーティクルの発生位置をボスの現在位置に設定
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());
		// パーティクルのサイズをボスの設定値に合わせる
		m_pParticle->SetBarrierBreakSize(pBoss->GetBarrierBreakSize());
		// パーティクルの更新
		m_pParticle->Update(elapsedTime);
	}
}
/*
*	@brief	描画
*	@param context デバイスコンテキスト
*	@param states ステート
*	@param world ワールド行列
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return	なし
*/
void BossSheild::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// m_pBossをBossクラスのポインタにキャスト
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);
	// シールドが展開されている間
	if (m_isSheild)
	{
		// シールドのワールド行列を作成（スケーリング＋親のワールド行列）
		Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;
		// シールド描画
		m_pSheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()
			{
				// 深度ステンシルステート設定（読み込みのみ）
				context->OMSetDepthStencilState(states->DepthRead(), 0);
				// 時計回りカリング設定（裏面をカリング）
				context->RSSetState(states->CullClockwise());
				// 加算合成ブレンドステート設定（発光エフェクト用）
				context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);
			});
	}
	// パーティクル再生フラグが立っている間
	if (m_isParticle)
	{
		// ボスのカメラ情報を使ってビルボード作成
		m_pParticle->CreateBillboard(pBoss->GetCameraTarget(), pBoss->GetCameraEye(), pBoss->GetCameraUp());
		// パーティクル描画
		m_pParticle->Render(view, proj);
	}
}

