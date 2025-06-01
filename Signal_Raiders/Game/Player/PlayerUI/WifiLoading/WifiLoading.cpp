/*
*	@file WifiLoading.cpp
*	@brief Wi-Fi���[�f�B���O�N���X
*/
#include <pch.h>
#include "WifiLoading.h"
/*
*	@brief �R���X�g���N�^
*	@details �E�B���h�E�̕��ƍ�����������
*	@param �Ȃ�
*	@return �Ȃ�
*/
WifiLoading::WifiLoading()
	: m_windowHeight(0)				// �E�B���h�E�̍���
	, m_windowWidth(0)				// �E�B���h�E�̕�
	, m_pDR(nullptr)				// �f�o�C�X���\�[�X
	, m_pCommonResources(nullptr)	// ���ʃ��\�[�X
	, m_pLoading(nullptr)			// ���[�f�B���OUI
	, m_pLoadgingText(nullptr)		// ���[�f�B���O�e�L�X�g
	, m_animSpeed(1)				// �A�j���[�V�����X�s�[�h
	, m_anim(0)						// �A�j���[�V�����J�E���g
	, m_animTime(0)					// �A�j���[�V��������
	, m_frameRows(1)				// �摜�̍s��
	, m_frameCols(4)				// �摜�̗�
{
}
/*
*	@brief �f�X�g���N�^
*	@details �����ł͉������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
WifiLoading::~WifiLoading() { /*do nothing*/ }
/*
*	@brief �������֐�
*	@details �f�o�C�X���\�[�X���擾���A�E�B���h�E�̕��ƍ�����ݒ��A���[�f�B���OUI���쐬����
*	@param resources �f�o�C�X���\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void WifiLoading::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_pCommonResources = resources;// ���ʃ��\�[�X��ݒ�
	m_windowWidth = width;// �E�B���h�E�̕�
	m_windowHeight = height;// �E�B���h�E�̍���
	// ���[�f�B���O�摜
	Add(m_pLoading, "Loading",
		DirectX::SimpleMath::Vector2((float)(m_windowWidth), (float)(m_windowHeight)),
		DirectX::SimpleMath::Vector2(.4f, .4f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ���[�f�B���O�e�L�X�g
	Add(m_pLoadgingText, "LoadingText",
		DirectX::SimpleMath::Vector2((float)(m_windowWidth - 70), (float)(m_windowHeight)),
		DirectX::SimpleMath::Vector2(.4f, .4f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ���[�f�B���O�摜�̃A�j���[�V�����ݒ�
	m_pLoading->SetFrameRows(m_frameRows);// �t���[���̍s����ݒ�
	m_pLoading->SetFrameCols(m_frameCols);// �t���[���̗񐔂�ݒ�
}
/*
*	@brief �X�V�֐�(�����p)
*	@details ���[�f�B���O�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void WifiLoading::Update(float elapsedTime)
{
	m_animTime += elapsedTime * m_animSpeed;// �o�ߎ��Ԃ����Z
	if (m_animTime > 1.25f)m_animTime = 0.0f;// �����Ԃ𒴂�����^�C�}�[�����Z�b�g
	float stepRatio = floor(m_animTime * m_frameCols) / m_frameCols;// 0.25 ���݂ɒ���
	m_pLoading->SetRenderRatio(stepRatio);// �A�j���[�V�����̔䗦��ݒ�
}
/*
*	@brief �`��֐�
*	@details ���[�f�B���O�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void WifiLoading::Render()
{
	m_pLoading->SetShaderType(PlayerUI::ShaderType::ANIM);// �V�F�[�_�[�^�C�v���A�j���[�V�����ɐݒ�
	m_pLoading->Render();// ���[�f�B���O�摜��`��
	m_pLoadgingText->SetShaderType(PlayerUI::ShaderType::OTHER);// �V�F�[�_�[�^�C�v�����̑��ɐݒ�
	m_pLoadgingText->Render();// ���[�f�B���O�e�L�X�g��`��
}
/*
*	@brief UI�̒ǉ��֐�
*	@details �w�肳�ꂽ�p�X��UI��ǉ�����
*	@param pPlayerUI �ǉ�����UI�̃X�}�[�g�|�C���^
*	@param key UI�̃e�N�X�`���L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void WifiLoading::Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);// PlayerUI�̃C���X�^���X�𐶐�
	// UI�̏�����
	pPlayerUI->Create(m_pDR	// �f�o�C�X���\�[�X
		, key				// �e�N�X�`���̃L�[
		, position			// UI�̈ʒu
		, scale				// UI�̃X�P�[��
		, anchor);			// UI�̃A���J�[��ݒ�
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ�
}
