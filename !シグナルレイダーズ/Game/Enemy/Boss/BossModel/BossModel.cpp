/*
	@file	BossModel.cpp
	@brief	�G���f���N���X
*/
#include "pch.h"
#include "Game/Enemy/Enemy.h"
#include "BossModel.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>


BossModel::BossModel()
	:
	m_commonResources{},
	m_bodyModel{},
	m_idlingFaceModel{},
	m_attackFaceModel{},
	m_nowState{ IState::EnemyState::IDLING }
{}

BossModel::~BossModel() {}

void BossModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Boss");
	// ���f����ǂݍ��ށi ���́A�\����j
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss.cmo", *fx);
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Idling.cmo", *fx);
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *fx);

}
void BossModel::Update(float elapsedTime, IState::EnemyState State)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_nowState = State;
}
void BossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bodyModel->Draw(context, *states, world, view, proj);// ����

	// �\���
	switch (m_nowState)
	{
	case IState::EnemyState::DAMAGE:// �_���[�W
	case IState::EnemyState::IDLING:// �A�C�h�����O
		m_idlingFaceModel->Draw(context, *states, world, view, proj);
		break;
	case IState::EnemyState::ATTACK:// �U��
	case IState::EnemyState::ANGRY:// �{��
		m_attackFaceModel->Draw(context, *states, world, view, proj);
		break;

	}
}