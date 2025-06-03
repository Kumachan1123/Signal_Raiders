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
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �}�E�X�|�C���^�[��UI���쐬
	m_pMousePointer = std::make_unique<UI>(m_pCommonResources);
	// �E�B���h�E�̕����Z�b�g
	m_windowWidth = width;
	// �E�B���h�E�̍������Z�b�g
	m_windowHeight = height;
	// �}�E�X�|�C���^�[��UI���쐬
	Add("MousePointer"//  �u�}�E�X�J�[�\���摜�v��ǂݍ���
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
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// �}�E�X�̍��W���擾
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// �}�E�X�̍��W���Z�b�g
	m_pMousePointer->SetPosition(mousePos);
}
/*
*	@brief	�`��
*	@detail �N���X�̕`�揈�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MousePointer::Render()
{
	// �}�E�X�|�C���^�[��UI��`��
	m_pMousePointer->Render();
}
/*
*	@brief	�A�C�e����ǉ�
*	@detail �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
*	@param std::string key �摜�̃L�[
*	@param DirectX::SimpleMath::Vector2 position �ʒu
*	@param DirectX::SimpleMath::Vector2 scale �X�P�[��
*	@param KumachiLib::ANCHOR anchor �A���J�[
*	@param IMenuUI::UIType type UI�̎��
*	@return �Ȃ�
*/
void MousePointer::Add(std::string key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, IMenuUI::UIType type)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(type);
	// �}�E�X�|�C���^�[��UI���쐬
	m_pMousePointer->Create(m_pDR// �f�o�C�X���\�[�X
		, key					// �摜�̃p�X
		, position				// �ʒu
		, scale					// �X�P�[��
		, anchor);				// �A���J�[
	// �E�B���h�E�̃T�C�Y���Z�b�g
	m_pMousePointer->SetWindowSize(m_windowWidth, m_windowHeight);
}
