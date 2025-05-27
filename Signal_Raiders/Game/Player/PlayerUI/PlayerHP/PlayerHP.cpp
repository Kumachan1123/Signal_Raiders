/*
*	@file PlayerHP.cpp
*	@brief HP�Q�[�W�N���X
*/
#include <pch.h>
#include "PlayerHP.h"
/*
*	@brief �R���X�g���N�^
*	@details ������
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayerHP::PlayerHP()
	: m_windowHeight(0)		// �E�B���h�E�̍���
	, m_windowWidth(0)		// �E�B���h�E�̕�
	, m_maxHP(100.0f)		// �ő�HP
	, m_currentHP(100.0f)	// ���݂�HP
	, m_pDR(nullptr)		// �f�o�C�X���\�[�X
	, m_pGauge(nullptr)		// HP�Q�[�W
	, m_pFrame(nullptr)		// HP�Q�[�W�̘g
	, m_pBack(nullptr)		// HP�Q�[�W�̔w�i
	, m_pHeartIcon(nullptr)	// �n�[�g�A�C�R��
{
}
/*
*	@brief �f�X�g���N�^
*	@details �����ł͉������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayerHP::~PlayerHP() {/*do nothing*/ }
/*
*	@brief �������֐�
*	@details UI�̏��������s��
*	@param resources �f�o�C�X���\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void PlayerHP::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pDR = resources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_windowWidth = width;// �E�B���h�E�̕���ݒ�
	m_windowHeight = height;// �E�B���h�E�̍�����ݒ�
	// HP�o�[
	Add(m_pGauge, L"Resources/Textures/HPBar.png"
		, Vector2(328, 40)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	m_pGauge->SetRenderRatioOffset(0);// �Q�[�W�̃I�t�Z�b�g��0�ɐݒ�
	// HP�o�[�̔w�i
	Add(m_pBack, L"Resources/Textures/HPBarBase.png"
		, Vector2(328, 40)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	// HP�o�[�̘g
	Add(m_pFrame, L"Resources/Textures/HPBarFrame.png"
		, Vector2(328, 40)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	// �n�[�g�A�C�R��
	Add(m_pHeartIcon, L"Resources/Textures/HP.png"
		, Vector2(0, -5)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::TOP_LEFT);
}
/*
*	@brief HP�X�V�֐�
*	@details �v���C���[��HP���X�V����
*	@param PlayerHP �v���C���[��HP
*	@return �Ȃ�
*/
void PlayerHP::Update(float PlayerHP)
{
	if (m_maxHP <= 0.0f) return; // �ő�HP��0�ȉ��ɂȂ�Ȃ��悤�`�F�b�N
	float hp = PlayerHP / m_maxHP; // �ő�HP���l�����Đ��K��
	hp = Clamp(hp, 0.0f, 1.0f); // 0.0�`1.0�͈̔͂Ɏ��߂�
	m_pGauge->SetRenderRatio(hp);// �Q�[�W�̔䗦��ݒ�
}
/*
*	@brief �`��֐�
*	@details UI��`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayerHP::Render()
{
	//�V�F�[�_�[�̎�ނ�ݒ肵�ĕ`��
	m_pBack->SetShaderType(PlayerUI::ShaderType::OTHER), m_pBack->Render();// �w�i��`��
	m_pGauge->SetShaderType(PlayerUI::ShaderType::HP), m_pGauge->Render();// �Q�[�W��`��
	m_pFrame->SetShaderType(PlayerUI::ShaderType::OTHER), m_pFrame->Render(); // �g��`��
	m_pHeartIcon->SetShaderType(PlayerUI::ShaderType::OTHER), m_pHeartIcon->Render();// �n�[�g�A�C�R����`��
}
/*
*	@brief UI�̒ǉ��֐�
*	@details PlayerUI��ǉ�����
*	@param pPlayerUI �ǉ�����PlayerUI�̃|�C���^
*	@param path �e�N�X�`���̃p�X
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void PlayerHP::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();// PlayerUI�̃C���X�^���X�𐶐�
	// UI�̏�����
	pPlayerUI->Create(m_pDR	// �f�o�C�X���\�[�X
		, path				// �e�N�X�`���̃p�X
		, position			// UI�̈ʒu
		, scale				// UI�̃X�P�[��
		, anchor);			// UI�̃A���J�[��ݒ�
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y��ݒ�
}