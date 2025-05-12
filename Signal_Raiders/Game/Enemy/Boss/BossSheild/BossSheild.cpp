/*
	@file	BossSheild.cpp
	@brief	ボスシールドクラス
*/
#include "pch.h"
#include "BossSheild.h"

/*
*	@brief	コンストラクタ
*	@return	なし
*/
BossSheild::BossSheild()
	: m_commonResources{}// 共通リソース
	, m_isSheild(false)// シールド展開フラグ
	, m_isParticle(false)// パーティクル再生フラグ
	, m_sheildSize(DirectX::SimpleMath::Vector3::Zero)// シールドのサイズ
	, m_sheildPosition(DirectX::SimpleMath::Vector3::Zero)// シールドの座標
	, m_sheildHP(0)// シールドのHP
	, m_bossType(BossShieldType::BOSS)// ボスの種類
	, m_pBoss(nullptr)// ボスクラスのポインタ
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
BossSheild::~BossSheild() {}

/*
*	@brief	シールドの初期化
*	@param[in] sheildHP シールドのHP
*	@param[in] pBoss ボスクラスのポインタ
*	@return	なし
*/
void BossSheild::SetUp(int sheildHP, IEnemy* pBoss)
{
	using namespace DirectX::SimpleMath;
	m_sheildHP = sheildHP;// シールドのHP
	m_pBoss = pBoss;// ボスクラスのポインタ
	m_isSheild = false;// シールド展開フラグを立てる
	m_sheildSize = Vector3::Zero;// シールドのサイズを初期化
	m_sheildPosition = pBoss->GetPosition();// ボスの位置をシールドの位置に設定
}

/*
*	@brief	初期化
*	@param[in] resources 共通リソース
*	@return	なし
*/
void BossSheild::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// 共通リソースを設定
	auto device = resources->GetDeviceResources()->GetD3DDevice();// デバイス取得
	m_pParticle = std::make_unique<Particle>(ParticleUtility::Type::BARRIERBREAK, 0.0f);// 0.0fなのはすでに内部で設定されているから
	m_pParticle->Initialize(m_commonResources);// パーティクル初期化
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);// エフェクトファクトリー作成
	fx->SetDirectory(L"Resources/Models/Boss");// ディレクトリ設定
	m_sheildModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Barrier.cmo", *fx);// モデル作成
}

/*
*	@brief	更新
*	@param[in] elapsedTime 経過時間
*	@return	なし
*/
void BossSheild::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);// ボスクラスのポインタを取得
	if (m_isSheild)// シールドが展開されている間
	{
		pBoss->PlayBarrierSE();// シールドSE再生
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3(0.4f), EnemyParameters::BOSS_SHIELD_SCALE_SPEED);// シールドのサイズを拡大
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefensiveHitRadius();// ボスの境界球をシールドの大きさに合わせる
		pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);// 攻撃の間隔を速くする
		pBoss->SetInitSpecialAttacCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN / 2);// 特殊攻撃の間隔を速くする
	}
	if (m_sheildHP <= 0)// シールドが破壊されたら
	{
		m_isParticle = true;// パーティクル再生
		m_isSheild = false;// シールド破壊
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefaultHitRadius();// ボスの境界球を元に戻す
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());// ボスの位置を設定
		m_pParticle->SetBarrierBreakSize(pBoss->GetBarrierBreakSize());
		m_pParticle->Update(elapsedTime);// パーティクル更新
	}
}
/*
*	@brief	描画
*	@param[in] context デバイスコンテキスト
*	@param[in] states ステート
*	@param[in] world ワールド行列
*	@param[in] view ビュー行列
*	@param[in] proj プロジェクション行列
*	@return	なし
*/
void BossSheild::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{

	using namespace DirectX::SimpleMath;

	// m_pBossをBossクラスのポインタにキャスト
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);
	if (m_isSheild)// シールドが展開されている間
	{
		Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;// シールドのワールド行列
		m_sheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()// シールド描画
			{
				context->OMSetDepthStencilState(states->DepthRead(), 0);// 深度ステンシルステート設定
				context->RSSetState(states->CullClockwise());// ラスタライザーステート設定
				context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);// ブレンドステート設定
			});
	}
	if (m_isParticle)// パーティクル再生フラグが立っている間
	{
		m_pParticle->CreateBillboard(pBoss->GetCameraTarget(), pBoss->GetCameraEye(), pBoss->GetCameraUp());// ビルボード作成
		m_pParticle->Render(view, proj);// パーティクル描画
	}
}
