/*
	@file	TitleMenu.h
	@brief	�^�C�g����ʂ̃��j���[�N���X
*/
#pragma once
//	�W�����C�u����
#include <vector>
// DirectX�̃��C�u����
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// �O�����C�u����
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include "Libraries/MyLib/InputManager.h"
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
// �O���錾
class CommonResources;
class MousePointer : public IMenuUI
{
public:
	// public�֐�
	MousePointer();// �R���X�g���N�^
	~MousePointer();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// ������
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); };// �X�V
	void Render()override;// �`��
	void Add(const wchar_t* path// UI�ǉ�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type)override;
private:
	// private�֐�
	void Update(float elapsedTime);// �X�V�iprivate�j
private:
	// private�ϐ�
	unsigned int m_menuIndex;// ���j���[�̃C���f�b�N�X
	DX::DeviceResources* m_pDR;// �f�o�C�X���\�[�X
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	std::unique_ptr<UI> m_pMousePointer;// �}�E�X�|�C���^�[
	int m_windowWidth, m_windowHeight;// �E�B���h�E�̕��ƍ���
	DirectX::Keyboard::KeyboardStateTracker m_tracker;// �L�[�{�[�h�̃g���b�J�[
};