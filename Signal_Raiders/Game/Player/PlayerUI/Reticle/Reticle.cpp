/*
*	@file Reticle.cpp
*	@brief �Ə��N���X
*/
#include <pch.h>
#include "Reticle.h"
/*
*	@brief �R���X�g���N�^
*	@details �E�B���h�E�̕��ƍ�����������
*	@param �Ȃ�
*	@return �Ȃ�
*/
Reticle::Reticle()
	: m_windowHeight(0)				// �E�B���h�E�̍���
	, m_windowWidth(0)				// �E�B���h�E�̕�
	, m_pDR(nullptr)				// �f�o�C�X���\�[�X
	, m_pCommonResources(nullptr)	// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pReticle(nullptr)			// �Ə�UI
{
}
/*
*	@brief �f�X�g���N�^
*	@details �����ł͉������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
Reticle::~Reticle() {/*do nothing*/ }
/*
*	@brief �������֐�
*	@details �f�o�C�X���\�[�X���擾���A�E�B���h�E�̕��ƍ�����ݒ��A�Ə�UI���쐬����
*	@param resources �f�o�C�X���\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Reticle::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	m_pDR = resources->GetDeviceResources();
	// �E�B���h�E�̕���ݒ�
	m_windowWidth = width;
	// �E�B���h�E�̍�����ݒ�
	m_windowHeight = height;
	// �Ə�UI�̒ǉ�
	Add(m_pReticle
		, "Reticle"
		, Vector2(640, 360)
		, Vector2(0.25f, 0.25f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}
/*
*	@brief �X�V�֐�(�O���p)
*	@details �Ə��̍X�V���s��(����UI�͕ω������Ȃ��̂ōX�V�����͂��Ȃ�)
*	@param context �X�V�R���e�L�X�g
*	@return �Ȃ�
*/
void Reticle::Update(const UpdateContext& context)
{
	// �X�V�R���e�L�X�g�͎g�p���Ȃ��̂Ŗ�������
	UNREFERENCED_PARAMETER(context);
}
/*
*	@brief �X�V�֐�(�����p)
*	@details �Ə��̍X�V���s��(����UI�͕ω������Ȃ��̂ōX�V�����͂��Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Reticle::Update() { /*do nothing*/ }
/*
*	@brief �`��֐�
*	@details �Ə��̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Reticle::Render()
{
	// �V�F�[�_�[�^�C�v��ݒ�
	m_pReticle->SetShaderType(PlayerUI::ShaderType::OTHER);
	// �Ə��̕`����s��
	m_pReticle->Render();
}
/*
*	@brief UI�̒ǉ��֐�
*	@details PlayerUI�̃C���X�^���X�𐶐����AUI�̏��������s��
*	@param pPlayerUI PlayerUI�̃C���X�^���X
*	@param key �e�N�X�`���̃L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void Reticle::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor)
{
	// PlayerUI�̃C���X�^���X�𐶐�
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);
	// UI�̏�����
	pPlayerUI->Create(m_pDR	// �f�o�C�X���\�[�X
		, key				// �e�N�X�`���̃p�X
		, position			// UI�̈ʒu
		, scale				// UI�̃X�P�[��
		, anchor);			// UI�̃A���J�[��ݒ�
	// �E�B���h�E�̃T�C�Y��ݒ�
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}



