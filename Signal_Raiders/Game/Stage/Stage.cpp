/*
	@file	Sky.cpp
	@brief	�X�J�C�N���X
*/
#include "pch.h"
#include "Stage.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Stage::Stage()
	:
	m_commonResources{},
	m_model{},
	m_texturePath{}
{
}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Stage::~Stage()
{
	// do nothing.
}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);
	m_commonResources = resources;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();


	// ���f����ǂݍ��ޏ���
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/models/Stage");
	//�e�N�X�`���p�X��ݒ肷��
	wcscpy_s(m_texturePath, L"Resources/models/Stage/Stage.cmo");

	// ���f����ǂݍ���
	m_model = Model::CreateFromCMO(device, m_texturePath, *fx);

}
//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	world *= Matrix::CreateTranslation(pos);
	// ���f���̃G�t�F�N�g�����X�V����
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// �ʂ̃��C�g�����ׂĖ���������
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);
				// ���f����������������
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
	// ���f����`�悷��
	m_model->Draw(context, *states, world, view, proj);
}
