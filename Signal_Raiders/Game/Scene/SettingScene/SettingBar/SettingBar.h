/*
*	@file	SettingBar.h
*	@brief	�ݒ�o�[�N���X�i���ʂ⊴�x�Ȃǂ̃X���C�_�[�j
*/
#pragma once
// �W�����C�u����
#include <vector>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <DeviceResources.h>
// �O�����C�u����
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class SettingBar : public IMenuUI
{
public:// �A�N�Z�T
	SettingMenu* GetSettingMenu() { return m_pSettingMenu; }// �ݒ胁�j���[�擾
	void SetSettingMenu(SettingMenu* menu) { m_pSettingMenu = menu; }// �ݒ胁�j���[�ݒ�
	void SetStateIDNum(SettingMenu::SettingID num) { m_num = num; }// ���ID�ݒ�
	int GetSetting(int index) const { return m_setting[index]; }// �ݒ�擾
public:// public�֐�
	SettingBar();// �R���X�g���N�^
	~SettingBar();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// ������
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// �X�V(context���玞�Ԃ��擾���čX�V)
	void Render()override;// �`��
	void Add(const wchar_t* path// ���j���[�A�C�e����ǉ�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type)override;
private:// private�֐�
	void Update(float elapsedTime);// �X�V
private:// private�萔
	static const DirectX::SimpleMath::Vector2 BGM_POSITION;	// BGM�̈ʒu
	static const DirectX::SimpleMath::Vector2 SE_POSITION;	// SE�̈ʒu
	static const DirectX::SimpleMath::Vector2 SENSITIVITY_POSITION;	// ���x�̈ʒu
	static const float BAR_WIDTH;	// �o�[�̕�
	static const float BAR_POINTER_DIVISION;// �ʂ̈ʒu�ɏ��Z����l
private:// private�ϐ�
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
	// �ݒ�o�[�e�N�X�`���p�X
	const wchar_t* m_pSettingBarTexturePath;
	// �ݒ�o�[�̋ʃe�N�X�`���p�X
	const wchar_t* m_pSettingBarPointerTexturePath;
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
	float m_time = 0;


};