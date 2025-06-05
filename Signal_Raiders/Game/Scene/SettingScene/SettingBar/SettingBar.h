/*
*	@file	SettingBar.h
*	@brief	�ݒ�o�[�N���X�i���ʂ⊴�x�Ȃǂ̃X���C�_�[�j
*/
#pragma once
#ifndef SETTING_BAR_DEFINED
#define SETTING_BAR_DEFINED
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// �O���錾
class CommonResources;

// �ݒ�o�[�N���X�i���ʂ⊴�x�Ȃǂ̃X���C�_�[�j
class SettingBar : public IMenuUI
{
public:
	// �A�N�Z�T
	// �ݒ胁�j���[�擾
	SettingMenu* GetSettingMenu() { return m_pSettingMenu; }
	// �ݒ胁�j���[�ݒ�
	void SetSettingMenu(SettingMenu* menu) { m_pSettingMenu = menu; }
	// ���ID�ݒ�
	void SetStateIDNum(SettingMenu::SettingID num) { m_num = num; }
	// �ݒ�擾
	int GetSetting(int index) const { return m_setting[index]; }
public:
	// public�֐�
	// �R���X�g���N�^
	SettingBar();
	// �f�X�g���N�^
	~SettingBar();
	// ������
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V(context���玞�Ԃ��擾���čX�V)
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }
	// �`��
	void Render()override;
	// ���j���[�A�C�e����ǉ�
	void Add(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, UIType type)override;
private:
	// private�֐�
	// �X�V
	void Update(float elapsedTime);
private:// private�萔
	// BGM�̈ʒu
	static const DirectX::SimpleMath::Vector2 BGM_POSITION;
	// SE�̈ʒu
	static const DirectX::SimpleMath::Vector2 SE_POSITION;
	// ���x�̈ʒu
	static const DirectX::SimpleMath::Vector2 SENSITIVITY_POSITION;
	// �o�[�̕�
	static const float BAR_WIDTH;
	// �ʂ̈ʒu�ɏ��Z����l
	static const float BAR_POINTER_DIVISION;
private:
	// private�ϐ�
	// �Z�b�e�B���O���j���[
	SettingMenu* m_pSettingMenu;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �ݒ�o�[
	std::vector<std::unique_ptr<UI>> m_pBar;
	// �ݒ�o�[�̋�
	std::vector<std::unique_ptr<UI>> m_pBarPointer;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �ݒ�f�[�^
	std::unique_ptr<SettingData> m_pSettingData;
	// �I��ԍ�
	SettingMenu::SelectID m_selectNum;
	// ���ID
	SettingMenu::SettingID m_num;
	// �ݒ�f�[�^
	unsigned int m_setting[3];
	// ����
	float m_time;
};
#endif // SETTING_BAR_DEFINED