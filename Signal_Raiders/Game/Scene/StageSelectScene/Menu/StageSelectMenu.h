/*
*	@file	StageSelectMenu.h
*	@brief	���U���g���j���[�N���X
*/
#pragma once
#ifndef STAGE_SELECT_MENU_DEFINED
#define STAGE_SELECT_MENU_DEFINED
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// �O���錾
class CommonResources;

// ���U���g���j���[�N���X
class StageSelectMenu : public IMenuUI
{
public:
	// �񋓌^
	// �V�[��ID
	enum SceneID
	{
		PLAY = 0,// �v���C�V�[��
		END// �^�C�g���ɖ߂�
	};
public:
	// �A�N�Z�T
	// �V�[���ԍ��擾
	SceneID GetSceneNum() const { return m_num; }
	// �V�[���ԍ��ݒ�
	void SetSceneNum(SceneID num) { m_num = num; }
	// ���j���[�C���f�b�N�X�擾
	int GetMenuIndex() const { return m_menuIndex; }
	// ���j���[�C���f�b�N�X�ݒ�
	void SetMenuIndex(int index) { m_menuIndex = index; }
	// UI�Ƀq�b�g�������ǂ����擾
	bool GetIsHit()const { return m_hit; }
	// UI�Ƀq�b�g�������ǂ����ݒ�
	void SetIsHit(bool hit) { m_hit = hit; }
	// SE�̉��ʐݒ�
	void SetSEVolume(float volume) { m_SEVolume = volume; }
public:
	// public�֐�
	// �R���X�g���N�^
	StageSelectMenu();
	// �f�X�g���N�^
	~StageSelectMenu();
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
private:
	// private�萔
	// �����ȃ��j���[�C���f�b�N�X
	static const int INVALID_MENU_INDEX;
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �I���\UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// �I��s�\UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// �I��UI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// �I��UI�̃e�N�X�`���p�X
	const wchar_t* m_pSelectTexturePath;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �I���\UI�̐�
	SceneID m_num;
	// ����
	float m_time;
	// �q�b�g�t���O
	bool m_hit;
	// �I���\UI�̃C���f�b�N�X
	unsigned int m_menuIndex;
	// SE�̉���
	float m_SEVolume;
	// �Đ��t���O
	bool m_isSEPlay;
};
#endif // STAGE_SELECT_MENU_DEFINED