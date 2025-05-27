/*
*	@file	TitleMenu.cpp
*	@brief	�^�C�g����ʂ̃��j���[�N���X
*/
#include <pch.h>
#include "MousePointer.h"
/*
*	@brief	�R���X�g���N�^
*	@detail �N���X�̐����������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
MousePointer::MousePointer()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_menuIndex{}// ���j���[�̃C���f�b�N�X
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pMousePointer{}// �}�E�X�|�C���^�[��UI
	, m_windowWidth{}// �E�B���h�E�̕�
	, m_windowHeight{}// �E�B���h�E�̍���
	, m_tracker{}// �L�[�{�[�h�̃g���b�J�[
{
}
/*
*	@brief	�f�X�g���N�^
*	@detail �N���X�̔j���������s��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
MousePointer::~MousePointer() {	/*do nothing.*/ }
/*
*	@brief	������
*	@detail �N���X�̏������������s��
*	@param CommonResources* resources ���ʃ��\�[�X
*	@param int width �E�B���h�E�̕�
*	@param int height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void MousePointer::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// ���ʃ��\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_pMousePointer = std::make_unique<UI>();// �}�E�X�|�C���^�[��UI���쐬
	m_windowWidth = width;// �E�B���h�E�̕����Z�b�g
	m_windowHeight = height;// �E�B���h�E�̍������Z�b�g
	// �}�E�X�|�C���^�[��UI���쐬
	Add(L"Resources/Textures/MousePointer.png"//  �u�}�E�X�J�[�\���摜�v��ǂݍ���
		, Vector2(0, 0)// �����ʒu
		, Vector2(1, 1)// �X�P�[��
		, KumachiLib::ANCHOR::MIDDLE_CENTER// �A���J�[
		, UIType::NON_SELECT);// �I��s��
}
/*
*	@brief	�X�V
*	@detail �N���X�̍X�V�������s��
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MousePointer::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedTime);// ���g�p�x����\��
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// �}�E�X�̏�Ԃ��擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// �}�E�X�̍��W���擾
	m_pMousePointer->SetPosition(mousePos);// �}�E�X�̍��W���Z�b�g
}
/*
*	@brief	�`��
*	@detail �N���X�̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MousePointer::Render() { m_pMousePointer->Render(); }
/*
*	@brief	�A�C�e����ǉ�
*	@detail �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
*	@param const wchar_t* path �摜�̃p�X
*	@param DirectX::SimpleMath::Vector2 position �ʒu
*	@param DirectX::SimpleMath::Vector2 scale �X�P�[��
*	@param KumachiLib::ANCHOR anchor �A���J�[
*	@param IMenuUI::UIType type UI�̎��
*	@return �Ȃ�
*/
void MousePointer::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, IMenuUI::UIType type)
{
	UNREFERENCED_PARAMETER(type);// ���g�p�x����\��
	m_pMousePointer->Create(m_pDR// �f�o�C�X���\�[�X
		, path					// �摜�̃p�X
		, position				// �ʒu
		, scale					// �X�P�[��
		, anchor);				// �A���J�[
	m_pMousePointer->SetWindowSize(m_windowWidth, m_windowHeight);// �E�B���h�E�̃T�C�Y���Z�b�g
}
