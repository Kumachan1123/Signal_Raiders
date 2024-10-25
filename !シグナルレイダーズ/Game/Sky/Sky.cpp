/*
	@file	Sky.cpp
	@brief	�X�J�C�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Game/Sky/Sky.h"
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
Sky::Sky(int StageID)
	:
	m_commonResources{},
	m_model{},
	m_texturePath{},
	m_stageID{ StageID }
{
}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Sky::~Sky()
{
	// do nothing.
}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Sky::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);
	m_commonResources = resources;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();


	// ���f����ǂݍ��ޏ���
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/models/sky");

	// �X�e�[�WID�ɂ���ăe�N�X�`���p�X����������
	switch (m_stageID)
	{

	case 1:
		//�e�N�X�`���p�X��ݒ肷��
		wcscpy_s(m_texturePath, L"Resources/models/sky/sky.cmo");
		break;
	case 2:
		//�e�N�X�`���p�X��ݒ肷��
		wcscpy_s(m_texturePath, L"Resources/models/sky/CloudySky.cmo");
		break;
	case 3:
		//�e�N�X�`���p�X��ݒ肷��
		wcscpy_s(m_texturePath, L"Resources/models/sky/EveningSky.cmo");
		break;
	case 4:
		//�e�N�X�`���p�X��ݒ肷��
		wcscpy_s(m_texturePath, L"Resources/models/sky/NightSky.cmo");
		break;
	case 5:
		//�e�N�X�`���p�X��ݒ肷��
		wcscpy_s(m_texturePath, L"Resources/models/sky/MidNightSky.cmo");
		break;
	}

	// ���f����ǂݍ���
	m_model = Model::CreateFromCMO(device, m_texturePath, *fx);

}
//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void Sky::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
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
