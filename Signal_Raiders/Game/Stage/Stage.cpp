/*
	@file	Sky.cpp
	@brief	�X�J�C�N���X
*/
#include "pch.h"
#include "Stage.h"

const float Stage::STAGE_SIZE = 98.5f;	// �X�e�[�W�̍L��
const float Stage::STAGE_HEIGHT = 3.0f;	// �X�e�[�W�̍���
/*
*	@brief	�R���X�g���N�^
*	@details �X�e�[�W�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Stage::Stage()
	: m_commonResources{}
	, m_model{}
	, m_texturePath{}
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �X�e�[�W�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Stage::~Stage() { /* do nothing.*/ }
/*
*	@brief	������
*	@details �X�e�[�W�̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	m_commonResources = resources;// ���\�[�X��ۑ�
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);// ���f����ǂݍ��ޏ���
	fx->SetDirectory(L"Resources/models/Stage");// ���f���̃f�B���N�g����ݒ�
	wcscpy_s(m_texturePath, L"Resources/models/Stage/Stage.cmo");//�e�N�X�`���p�X��ݒ肷��
	m_model = Model::CreateFromCMO(device, m_texturePath, *fx);// ���f����ǂݍ���
	m_model->UpdateEffects([](DirectX::IEffect* effect)	// ���f���̃G�t�F�N�g�����X�V����
		{

			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			if (!basicEffect)return;// �G�t�F�N�g��nullptr�̏ꍇ�͏������I����
			basicEffect->SetLightEnabled(0, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(1, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(2, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetEmissiveColor(Colors::White);// ���f����������������
		}
	);
}
/*
*	@brief	�`��
*	@details �X�e�[�W�̕`��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@param world ���[���h�s��
*	@param pos ���[���h���W
*	@return �Ȃ�
*/
void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_commonResources->GetCommonStates();// ���ʃX�e�[�g���擾
	world *= Matrix::CreateTranslation(pos);// ���[���h�s����X�V
	m_model->Draw(context, *states, world, view, proj);// ���f����`�悷��
}