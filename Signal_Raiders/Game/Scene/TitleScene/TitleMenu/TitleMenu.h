/*
*	@file	TitleMenu.h
*	@brief	���j���[�N���X
*/
#pragma once
#ifndef TITLE_MENU_DEFINED
#define TITLE_MENU_DEFINED
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <Mouse.h>
#include <WICTextureLoader.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// �O���錾
class CommonResources;

// ���j���[�N���X
class TitleMenu : public IMenuUI
{
public:
	// �\����
	// UI�̕ό`
	struct UITransForm
	{
		DirectX::SimpleMath::Vector2 position;// �ʒu
		DirectX::SimpleMath::Vector2 scale;// �X�P�[��
	};
	// �V�[���ԍ�
	enum SceneID
	{
		STAGESELECT = 0,// �X�e�[�W�I��
		SETTING,// �ݒ���
		END// �Q�[���I��
	};
public:
	// �A�N�Z�T
	// �V�[���ԍ��擾
	SceneID GetSceneNum() const { return m_num; }
	// �V�[���ԍ��ݒ�
	void SetSceneNum(SceneID num) { m_num = num; }
	// UI�Ƀq�b�g�������ǂ����擾
	bool GetIsHit()const { return m_hit; }
	// UI�Ƀq�b�g�������ǂ����ݒ�
	void SetIsHit(bool hit) { m_hit = hit; }
	// SE�̉��ʐݒ�
	void SetSEVolume(float volume) { m_SEVolume = volume; }
public:
	// public�֐�
	// �R���X�g���N�^
	TitleMenu();
	// �f�X�g���N�^
	~TitleMenu();
	// ������
	void Initialize(CommonResources* resources, int width, int height);
	// �X�V
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }
	// �`��
	void Render();
	// UI�ǉ�
	void Add(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type);
private:
	// private�֐�
	// �X�V
	void Update(float elapsedTime);
private:
	// private�萔
	// �����ȃ��j���[�C���f�b�N�X
	static const int INVALID_MENU_INDEX;
private:
	// private�ϐ�
	// ���j���[�̃C���f�b�N�X
	unsigned int m_menuIndex;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �I���\UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// �I��s�\UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// �I�𒆂�UI�ɕ\������摜
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// �I�𒆂�UI�ɕ\������摜�̃p�X
	const wchar_t* m_pSelectTexturePath;
	// �E�B���h�E�̕��E���� 
	int m_windowWidth, m_windowHeight;
	// UI�̍��W�Ȃǂ̏����i�[����z��
	std::vector<UITransForm> m_transforms;
	// �V�[���ԍ�
	SceneID m_num;
	// ����
	float m_time;
	// �����������ǂ���
	bool m_hit;
	// SE�̉���
	float m_SEVolume;
	// �Đ��t���O
	bool m_isSEPlay;
};
#endif // TITLE_MENU_DEFINED