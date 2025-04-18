/*
	@file	BossSheild.cpp
	@brief	ボスシールドクラス
*/
#include "pch.h"
#include "BossSheild.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief	コンストラクタ
*	@param sheildHP シールドの耐久値
*	@param pBoss ボスクラスのポインタ
*	@return	なし
*/
BossSheild::BossSheild(int sheildHP, Boss* pBoss)
	: m_commonResources{}// 共通リソース
	, m_isSheild(false)// シールド展開フラグ
	, m_isParticle(false)// パーティクル再生フラグ
	, m_sheildSize(Vector3::Zero)// シールドのサイズ
	, m_sheildPosition(Vector3::Zero)// シールドの座標
	, m_sheildHP(sheildHP)// シールドのHP
	, m_pBoss(pBoss)// ボスクラスのポインタ
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
BossSheild::~BossSheild() {}

/*
*	@brief	初期化
*	@param resources 共通リソース
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

void BossSheild::Update(float elapsedTime)
{
	if (m_isSheild)// シールドが展開されている間
	{
		m_pBoss->PlayBarrierSE();// シールドSE再生
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3::One, EnemyParameters::BOSS_SHIELD_SCALE_SPEED);// シールドのサイズを拡大
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::BOSS_SHIELD_RADIUS;// ボスの境界球をシールドの大きさに合わせる
		m_pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);// 攻撃の間隔を速くする
		m_pBoss->SetInitSpecialAttacCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN / 2);// 特殊攻撃の間隔を速くする
	}
	if (m_sheildHP <= 0)// シールドが破壊されたら
	{
		m_isParticle = true;// パーティクル再生
		m_isSheild = false;// シールド破壊
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::NORMAL_BOSS_RADIUS;// ボスの境界球を元に戻す
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());// ボスの位置を設定
		m_pParticle->Update(elapsedTime);// パーティクル更新
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
void BossSheild::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, Matrix world, Matrix view, Matrix proj)
{
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
		m_pParticle->CreateBillboard(m_pBoss->GetCameraTarget(), m_pBoss->GetCameraEye(), m_pBoss->GetCameraUp());// ビルボード作成
		m_pParticle->Render(view, proj);// パーティクル描画
	}
}
