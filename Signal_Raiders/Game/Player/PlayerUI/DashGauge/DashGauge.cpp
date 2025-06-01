/*
*	@file DashGauge.cpp
*	@brief �_�b�V���Q�[�W�N���X
*/
#include <pch.h>
#include "DashGauge.h"
/*
*	@brief �R���X�g���N�^
*	@details �_�b�V���Q�[�W�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
DashGauge::DashGauge()
	: m_windowHeight(0)	// �E�B���h�E�̍���
	, m_windowWidth(0)	// �E�B���h�E�̕�
	, m_pDR(nullptr)	// �f�o�C�X���\�[�X�ւ̃|�C���^
	, m_pCommonResources(nullptr)	// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pGauge(nullptr)	// �Q�[�W�{��
	, m_pFrame(nullptr)	// �Q�[�W�̘g
	, m_pBack(nullptr)	// �Q�[�W�̔w�i
	, m_pDashIcon(nullptr)	// �_�b�V���A�C�R��
{
}
/*
*	@brief �f�X�g���N�^
*	@details �_�b�V���Q�[�W�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
DashGauge::~DashGauge() {/*do nothing*/ }
/*
*	@brief ������
*	@details �_�b�V���Q�[�W�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void DashGauge::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources; // ���ʃ��\�[�X��ݒ�
	m_pDR = resources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_windowWidth = width;// �E�B���h�E�̕���ݒ�
	m_windowHeight = height;// �E�B���h�E�̍�����ݒ�
	Add(m_pBack,// �Q�[�W�̔w�i
		"HPBarBase",
		Vector2(210, 100),
		Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	Add(m_pFrame,// �Q�[�W�̘g
		"HPBarFrame",
		Vector2(210, 100),
		Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	Add(m_pGauge,// �Q�[�W�{��
		"HPBar",
		Vector2(210, 100),
		Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	m_pGauge->SetRenderRatioOffset(0);
	Add(m_pDashIcon,// �_�b�V���A�C�R��
		"Dash",
		Vector2(-5, 75),
		Vector2(1.0f, 1.0f),
		KumachiLib::ANCHOR::TOP_LEFT);
}
/*
*	@brief �X�V
*	@details �_�b�V���Q�[�W�̍X�V���s��
*	@param dashStamina �_�b�V���X�^�~�i�̒l
*	@return �Ȃ�
*/
void DashGauge::Update(float dashStamina)
{
	float stamina = dashStamina * 0.1f;// �_�b�V���X�^�~�i��0.1�{���Đ��K��
	stamina = std::max(0.f, stamina); // 0�����ɂȂ�Ȃ��悤�Ƀ`�F�b�N
	m_pGauge->SetRenderRatio(stamina);// �Q�[�W�̕\���䗦��ݒ�
}
/*
*	@brief �`��
*	@details �_�b�V���Q�[�W�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DashGauge::Render()
{
	//�V�F�[�_�[�̎�ނ�ݒ肵�ĕ`��
	m_pBack->SetShaderType(PlayerUI::ShaderType::OTHER), m_pBack->Render();// �w�i��`��
	m_pGauge->SetShaderType(PlayerUI::ShaderType::HP), m_pGauge->Render(); // �Q�[�W�{�̂�`��
	m_pFrame->SetShaderType(PlayerUI::ShaderType::OTHER), m_pFrame->Render(); // �g��`��
	m_pDashIcon->SetShaderType(PlayerUI::ShaderType::OTHER), m_pDashIcon->Render();// �_�b�V���A�C�R����`��
}
/*
*	@brief UI�ǉ�
*	@details �_�b�V���Q�[�W��UI��ǉ�����
*	@param pPlayerUI �ǉ�����UI�̃X�}�[�g�|�C���^
*	@param key UI�̃e�N�X�`���L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void DashGauge::Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);// �V����PlayerUI���쐬
	// PlayerUI���쐬���A�w�肳�ꂽ�p�X�A�ʒu�A�X�P�[���A�A���J�[�ŏ�����
	pPlayerUI->Create(m_pDR
		, key
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}