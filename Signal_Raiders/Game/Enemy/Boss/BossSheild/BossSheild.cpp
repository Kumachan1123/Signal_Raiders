/*
	@file	BossSheild.cpp
	@brief	ボスシールドクラス
*/
#include "pch.h"
#include "BossSheild.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include "Libraries/Microsoft/ReadData.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

BossSheild::BossSheild(int sheildHP, Boss* pBoss)
	: m_commonResources{}
	, m_isSheild{ false }
	, m_isParticle{ false }
	, m_sheildSize{ 0,0,0 }
	, m_sheildPosition{ 0,0,0 }
	, m_sheildHP{ sheildHP }// シールドの耐久値
	, m_pBoss{ pBoss }

{
}

BossSheild::~BossSheild()
{
}

void BossSheild::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// パーティクル生成
	m_pParticle = std::make_unique<Particle>(ParticleUtility::Type::BARRIERDESTROYED, 0.0f);// 0.0fなのはすでに内部で設定されているから
	m_pParticle->Initialize(m_commonResources);

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Boss");
	// シールドモデルを読み込む
	m_sheildModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Barrier.cmo", *fx);
}

void BossSheild::Update(float elapsedTime)
{
	if (m_isSheild)// シールドが展開されている間
	{
		m_pBoss->PlayBarrierSE();// シールドSE再生
		// シールドが展開されている間はサイズを拡大
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3::One, EnemyParameters::BOSS_SHIELD_SCALE_SPEED);
		// ボスの境界球を大きくする
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::BOSS_SHIELD_RADIUS;
		// 攻撃の間隔を速くする
		m_pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);
	}
	// シールドが破壊されたらパーティクルを再生
	if (m_sheildHP <= 0)
	{
		m_isParticle = true;// パーティクル再生
		m_isSheild = false;// シールド破壊
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::NORMAL_BOSS_RADIUS;// ボスの境界球を元に戻す
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());// ボスの位置を設定
		m_pParticle->Update(elapsedTime);// パーティクル更新
	}
}

void BossSheild::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, Matrix world, Matrix view, Matrix proj)
{
	if (m_isSheild)
	{
		// シールド用のスケール行列を適用
		Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;

		// シールド
		m_sheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()
			{
				context->OMSetDepthStencilState(states->DepthRead(), 0);
				context->RSSetState(states->CullClockwise());
				context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);
			});
	}

	// パーティクル描画
	if (m_isParticle)
	{
		m_pParticle->CreateBillboard(m_pBoss->GetCameraTarget(), m_pBoss->GetCameraEye(), m_pBoss->GetCameraUp());
		m_pParticle->Render(view, proj);
	}
}
