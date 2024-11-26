/*
	@file	BossModel.cpp
	@brief	敵モデルクラス
*/
#include "pch.h"
#include "Game/Enemy/Boss/Boss.h"
#include "BossModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include "Libraries/Microsoft/ReadData.h"
#include "Game/KumachiLib/KumachiLib.h"
BossModel::BossModel()
	: m_commonResources{}
	, m_bodyModel{}
	, m_pixelShader{}
	, m_idlingFaceModel{}
	, m_attackFaceModel{}
	, m_isSheild{ false }
	, m_sheildSize{ 0,0,0 }
	, m_nowState{ IState::EnemyState::IDLING }
{}

BossModel::~BossModel() {}

void BossModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// 影用のシェーダーを読み込む
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_EnemyShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Boss");
	// モデルを読み込む（ 胴体、表情差分）
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss.cmo", *fx);
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Idling.cmo", *fx);
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *fx);
	//m_sheildModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Barrier.cmo", *fx);
}
void BossModel::Update(float elapsedTime, IState::EnemyState State)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_nowState = State;
	//if (m_isSheild)
	//	m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3::One, 0.2f);
}
void BossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	shadowMatrix = world * shadowMatrix;
	// 影描画
	m_bodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			context->RSSetState(states->CullClockwise());
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});
	m_bodyModel->Draw(context, *states, world, view, proj);// 胴体
	//if (m_isSheild)
	//{
	//	// シールド用のスケール行列を適用
	//	Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;

	//	// シールド
	//	m_sheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()
	//		{
	//			context->OMSetDepthStencilState(states->DepthRead(), 0);
	//			context->RSSetState(states->CullClockwise());
	//			context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);
	//		});
	//}

	// 表情差分
	switch (m_nowState)
	{
	case IState::EnemyState::DAMAGE:// ダメージ
	case IState::EnemyState::IDLING:// アイドリング
		m_idlingFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ATTACK:// 攻撃
	case IState::EnemyState::ANGRY:// 怒り
		m_attackFaceModel->Draw(context, *states, world, view, proj);
		break;

	}
}
