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
	m_pCommonResources = resources; // ���ʃ��\�[�X��ݒ�
	m_pDR = resources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_windowWidth = width;// �E�B���h�E�̕���ݒ�
	m_windowHeight = height;// �E�B���h�E�̍�����ݒ�
	Add(m_pReticle// �Ə�UI�̒ǉ�
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
	UNREFERENCED_PARAMETER(context);// �X�V�R���e�L�X�g�͎g�p���Ȃ��̂Ŗ�������
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
	m_pReticle->SetShaderType(PlayerUI::ShaderType::OTHER);// �V�F�[�_�[�^�C�v��ݒ�
	m_pReticle->Render();// �Ə��̕`����s��
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
void Reticle::Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);// PlayerUI�̃C���X�^���X�𐶐�
	// UI�̏�����
	pPlayerUI->Create(m_pDR	// �f�o�C�X���\�[�X
		, key				// �e�N�X�`���̃p�X
		, position			// UI�̈ʒu
		, scale				// UI�̃X�P�[��
		, anchor);			// UI�̃A���J�[��ݒ�
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ�
}



