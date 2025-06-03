/*
*	@file	SettingMenu.h
*	@brief	�Z�b�e�B���O���j���[�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <Mouse.h>
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/CommonResources.h"
#include "Game/Interface/IMenuUI.h"
// �O���錾
class CommonResources;
class SettingMenu : public IMenuUI
{
public:// �񋓌^
	enum SettingID// �ݒ荀�� 
	{
		BGM = 0,// BGM
		SE = 1,// SE
		MOUSE = 2// �}�E�X���x

	};
	enum SelectID// �I������
	{
		NONE = -1,// �I���Ȃ�
		APPLY = 0,// �K�p
		END = 1// ���������I��
	};
public:// �A�N�Z�T
	SelectID GetSelectIDNum() const { return m_selectNum; }// �I��ID�擾
	void SetSelectIDNum(SelectID num) { m_selectNum = num; }// �I��ID�ݒ�
	unsigned int GetMenuIndex() const { return m_menuIndex; }// ���j���[�C���f�b�N�X�擾	
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SE�̉��ʐݒ�

public:// public�֐�
	SettingMenu();// �R���X�g���N�^
	~SettingMenu();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// ������
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// �X�V(context���玞�Ԃ��擾���čX�V)
	void Render()override;// �`��
	void Add(const std::string& key// ���j���[�A�C�e����ǉ�
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, UIType type);
private:// private�֐�
	void Update(float elapsedTime);// �X�V
private:// private�萔
	static const int INVALID_MENU_INDEX;// �����ȃ��j���[�C���f�b�N�X
private://	�ϐ�
	// ���j���[�̃C���f�b�N�X
	int m_menuIndex;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �I���\UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// �I��s�\UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// �I���\UI�ɂ�UI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// �I���e�N�X�`���p�X
	const wchar_t* m_pSelectTexturePath;
	// �I���E�B���h�E
	std::unique_ptr<UI> m_pSelectWindow;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �I��ID
	SelectID m_selectNum;
	// ����
	float m_time;
	// SE�̉���
	float m_SEVolume;
	// �Đ��t���O
	bool m_isSEPlay;
};