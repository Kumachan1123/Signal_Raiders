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
	: m_windowHeight(Screen::UI_HEIGHT)
	, m_windowWidth(Screen::UI_WIDTH)
	, m_pDR(nullptr)
	, m_pPlayGuide(nullptr)
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
	m_pDR = resources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_windowWidth = width;// �E�B���h�E�̕���ݒ�
	m_windowHeight = height;// �E�B���h�E�̍�����ݒ�
	CreatePlayerUI(L"Resources/Textures/PlayGuide.png"// �v���C��������̃e�N�X�`���p�X
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
	UNREFERENCED_PARAMETER(context);// �X�V�R���e�L�X�g�͎g�p���Ȃ��̂Ŗ�������
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
	m_pPlayGuide->SetShaderType(PlayerUI::ShaderType::OTHER);// �V�F�[�_�[�^�C�v��ݒ�
	m_pPlayGuide->Render();// �v���C��������̕`����s��
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
void PlayGuide::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
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
/*
*	@brief UI�쐬�֐�
*	@details �v���C���������UI���쐬����
*	@param path �e�N�X�`���̃p�X
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void PlayGuide::CreatePlayerUI(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	Add(m_pPlayGuide, path, position, scale, anchor);// UI��ǉ�����֐����Ăяo��
}


