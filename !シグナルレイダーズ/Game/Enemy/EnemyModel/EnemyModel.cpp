/*
	@file	EnemyModel.cpp
	@brief	�G���f���N���X
*/
#include "pch.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>


EnemyModel::EnemyModel()
	:
	m_commonResources{},
	m_bodyModel{},
	m_antennaModel{},
	m_attackFaceModel{},
	m_handModel{},
	m_idlingFaceModel{},
	m_angryFaceModel{},
	m_damageFaceModel{},
	m_nowState{ IState::EnemyState::IDLING }
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
	// ���f����ǂݍ��ށi ���A�A���e�i�A��A�\����j
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Head.cmo", *fx);
	m_antennaModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *fx);
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);
	m_handModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *fx);
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);
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
	m_bodyModel->Draw(context, *states, world, view, proj);// ��
	m_antennaModel->Draw(context, *states, world, view, proj);// �A���e�i
	// ��
	m_handModel->Draw(context, *states, world, view, proj);
	// �\���
	switch (m_nowState)
	{
	case IState::EnemyState::IDLING:// �A�C�h�����O
		m_idlingFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ATTACK:// �U��
		m_attackFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ANGRY:// �{��
		m_angryFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::DAMAGE:// ��e
		m_damageFaceModel->Draw(context, *states, world, view, proj);
		break;

	}
}