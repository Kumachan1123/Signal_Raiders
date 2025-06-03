/*
*	@file	TitleMenu.h
*	@brief	�^�C�g����ʂ̃��j���[�N���X
*/
#pragma once
//	�W�����C�u����
#include <vector>
#include <string>
// DirectX�̃��C�u����
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
// �O���錾
class CommonResources;
class MousePointer : public IMenuUI
{
public:// public�֐�
	// �R���X�g���N�^
	MousePointer();
	// �f�X�g���N�^
	~MousePointer();
	// ������
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }
	// �`��
	void Render()override;
	// UI�ǉ�
	void Add(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type)override;
private:// private�֐�
	// �X�V�iprivate�j
	void Update(float elapsedTime);
private:// private�ϐ�
	// ���j���[�̃C���f�b�N�X
	unsigned int m_menuIndex;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �}�E�X�|�C���^�[
	std::unique_ptr<UI> m_pMousePointer;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
};