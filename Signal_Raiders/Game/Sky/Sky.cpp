/*
	@file	Sky.cpp
	@brief	�X�J�C�N���X
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
#include <locale> 
#include <codecvt>
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


	auto it = m_texturePathMap.find(m_stageID);
	if (it != m_texturePathMap.end()) {
		std::wstring wpath = ConvertToWString(it->second);
		m_model = Model::CreateFromCMO(device, wpath.c_str(), *fx);
	}


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

std::wstring Sky::ConvertToWString(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}
