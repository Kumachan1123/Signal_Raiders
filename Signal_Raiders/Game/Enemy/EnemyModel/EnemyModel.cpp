/*
	@file	EnemyModel.cpp
	@brief	敵モデルクラス
*/
#include "pch.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include "Libraries/Microsoft/ReadData.h"

EnemyModel::EnemyModel()
	: m_commonResources{}
	, m_bodyModel{}
	, m_antennaModel{}
	, m_attackFaceModel{}
	, m_handModel{}
	, m_idlingFaceModel{}
	, m_angryFaceModel{}
	, m_damageFaceModel{}
	, m_shadowModel{}
	, m_nowState{ IState::EnemyState::IDLING }
{}

EnemyModel::~EnemyModel() {}

void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// 影用のシェーダーを読み込む
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_EnemyShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));
	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Enemy");
	// モデルを読み込む（ 頭、アンテナ、手、表情差分）
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Head.cmo", *fx);
	m_antennaModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *fx);
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);
	m_handModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *fx);
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);
	// 影用のモデルを読み込む
	m_shadowModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
}
void EnemyModel::Update(float elapsedTime, IState::EnemyState State)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_nowState = State;
}
void EnemyModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bodyModel->Draw(context, *states, world, view, proj);// 頭
	m_antennaModel->Draw(context, *states, world, view, proj);// アンテナ
	// 手
	m_handModel->Draw(context, *states, world, view, proj);
	// ライトの方向
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	shadowMatrix = world * shadowMatrix;
	// 影描画
	m_shadowModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			context->RSSetState(states->CullNone());
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});
	// 表情差分
	switch (m_nowState)
	{
	case IState::EnemyState::IDLING:// 徘徊
		m_idlingFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ATTACK:// 攻撃
		m_attackFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ANGRY:// 怒り
		m_angryFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::HIT:// 被弾中
		m_damageFaceModel->Draw(context, *states, world, view, proj);
		break;

	}
}