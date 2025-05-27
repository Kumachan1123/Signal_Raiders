/*
*	@file	Sky.cpp
*	@brief	�X�J�C�N���X
*/
#include "pch.h"
#include "Sky.h"
/*
*	@brief	�R���X�g���N�^
*	@details �X�J�C�̃R���X�g���N�^
*	@param StageID �X�e�[�WID
*	@return �Ȃ�
*/
Sky::Sky(int StageID)
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_model{}// ���f��
	, m_texturePath{}// �e�N�X�`���p�X
	, m_stageID{ StageID }// �X�e�[�WID
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �X�J�C�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Sky::~Sky() {/*/do nothing.*/ }
/*
*	@brief	������
*	@details ��̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Sky::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	m_pCommonResources = resources;// ���\�[�X��ۑ�
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);// ���f����ǂݍ��ޏ���
	fx->SetDirectory(L"Resources/models/sky");// ���f���̃f�B���N�g����ݒ�
	auto it = m_texturePathMap.find(m_stageID);// �X�e�[�WID�ɉ�������̃��f���̃p�X���擾
	if (it != m_texturePathMap.end()) // �}�b�v�̗v�f�����������ꍇ
	{
		std::wstring wpath = ConvertToWString(it->second);// �X�e�[�WID�ɉ�������̃��f���̃p�X���擾
		m_model = Model::CreateFromCMO(device, wpath.c_str(), *fx);// ���f����ǂݍ���
	}
	m_model->UpdateEffects([](DirectX::IEffect* effect)	// ���f���̃G�t�F�N�g�����X�V����
		{
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			if (!basicEffect)return;// �G�t�F�N�g��nullptr�̏ꍇ�͏������I����
			basicEffect->SetLightEnabled(0, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(1, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(2, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetEmissiveColor(Colors::White);// �G�~�b�V�u�J���[��ݒ肷��
		}
	);
}
/*
*	@brief	�`��
*	@details �X�J�C�̕`��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@param world ���[���h�s��
*	@param pos �X�J�C�̈ʒu
*	@return �Ȃ�
*/
void Sky::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_pCommonResources->GetCommonStates();// ���ʃX�e�[�g���擾
	world *= Matrix::CreateTranslation(pos);// ���[���h�s����X�V
	m_model->Draw(context, *states, world, view, proj);// ���f����`�悷��
}
/*
*	@brief	������ϊ�
*	@details ����������C�h������ɕϊ�����
*	@param str �ϊ����镶����
*	@return ���C�h������
*/
std::wstring Sky::ConvertToWString(const std::string& str) { return std::wstring(str.begin(), str.end()); }