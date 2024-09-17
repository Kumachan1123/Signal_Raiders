/*
	@file	SkyBox.cpp
	@brief	�X�J�C�{�b�N�X�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Game/SkyBox/SkyBox.h"
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
SkyBox::SkyBox()
	:
	m_commonResources{},
	m_model{}
{
}
//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
SkyBox::~SkyBox()
{
	// do nothing.
}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void SkyBox::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);
	m_commonResources = resources;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/sky");

	// ���f����ǂݍ���
	m_model = Model::CreateFromCMO(device, L"Resources/Models/sky/sky.cmo", *fx);

}
//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void SkyBox::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
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
								   //// ���C�g��L��������
								   //basicEffect->SetLightingEnabled(true);
								   /*
									   �E���f���N���X�ł̓f�t�H���g�œ����I�Ƀ��C�g���L��������Ă���
									   �E���̐ݒ肪�����Ă����C�g�͗L��������Ă���
								   */

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
