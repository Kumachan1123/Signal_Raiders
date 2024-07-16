/*
	@file	Enemy.cpp
	@brief	敵クラス
	作成者：くまち
*/
#include "pch.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>


EnemyModel::EnemyModel()
	:
	m_commonResources{}
{}
EnemyModel::~EnemyModel() {}

void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Enemy");
	// モデルを読み込む（今は仮で分解前のモデル）
	m_headModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
	/*m_antennaModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *fx);
	m_faceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);
	m_handModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *fx);*/

}
void EnemyModel::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

}
void EnemyModel::Render(ID3D11DeviceContext1* context,
						DirectX::DX11::CommonStates* states,
						DirectX::SimpleMath::Matrix world,
						DirectX::SimpleMath::Matrix view,
						DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_headModel->Draw(context, *states, world, view, proj);
	/*m_antennaModel->Draw(context, *states, world, view, proj);
	m_faceModel->Draw(context, *states, world, view, proj);
	m_handModel->Draw(context, *states, world, view, proj);*/
}