/*
*	@file PlayGuide.cpp
*	@brief �v���C��������N���X
*/
#include <pch.h>
#include "PlayGuide.h"
/*
*	@brief �R���X�g���N�^
*	@details �E�B���h�E�̕��ƍ�����������
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayGuide::PlayGuide()
	: m_windowHeight(Screen::UI_HEIGHT)	// �E�B���h�E�̍���
	, m_windowWidth(Screen::UI_WIDTH)	// �E�B���h�E�̕�
	, m_pCommonResources(nullptr)		// ���ʃ��\�[�X�ւ̃|�C���^
	, m_pDR(nullptr)					// �f�o�C�X���\�[�X�ւ̃|�C���^
	, m_pPlayGuide(nullptr)				// �v���C���������UI�|�C���^
{
}
/*
*	@brief �f�X�g���N�^
*	@details �����ł͉������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlayGuide::~PlayGuide() {/*do nothing*/ }
/*
*	@brief �������֐�
*	@details �f�o�C�X���\�[�X���擾���A�E�B���h�E�̕��ƍ�����ݒ��AUI���쐬����
*	@param resources �f�o�C�X���\�[�X
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void PlayGuide::Initialize(CommonResources* resources, int width, int height)
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
	// �v���C��������̃e�N�X�`���p�X
	CreatePlayerUI("PlayGuide"
		, Vector2(Screen::UI_LEFT, Screen::UI_BOTTOM)
		, Vector2(0.5f, 0.5f)
		, KumachiLib::ANCHOR::BOTTOM_LEFT);
}
/*
*	@brief �X�V�֐�(�O���p)
*	@details �v���C��������̍X�V���s��(����UI�͕ω������Ȃ��̂ōX�V�����͂��Ȃ�
*	@param context �X�V�R���e�L�X�g
*	@return �Ȃ�
*/
void PlayGuide::Update(const UpdateContext& context)
{
	// �X�V�R���e�L�X�g�͎g�p���Ȃ��̂Ŗ�������
	UNREFERENCED_PARAMETER(context);
}
/*
*	@brief �X�V�֐�(�����p)
*	@details �v���C��������̍X�V���s��(����UI�͕ω������Ȃ��̂ōX�V�����͂��Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayGuide::Update() {/*do nothing*/ }
/*
*	@brief �`��֐�
*	@details �v���C��������̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlayGuide::Render()
{
	// �V�F�[�_�[�^�C�v��ݒ�
	m_pPlayGuide->SetShaderType(PlayerUI::ShaderType::OTHER);
	// �v���C��������̕`����s��
	m_pPlayGuide->Render();
}
/*
*	@brief UI�̒ǉ��֐�
*	@details PlayerUI��ǉ�����
*	@param pPlayerUI �ǉ�����PlayerUI�̃|�C���^
*	@param key �e�N�X�`���̃L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void PlayGuide::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor)
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
/*
*	@brief UI�쐬�֐�
*	@details �v���C���������UI���쐬����
*	@param key �e�N�X�`���̃L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void PlayGuide::CreatePlayerUI(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor)
{
	// UI��ǉ�����֐����Ăяo��
	Add(m_pPlayGuide, key, position, scale, anchor);
}


