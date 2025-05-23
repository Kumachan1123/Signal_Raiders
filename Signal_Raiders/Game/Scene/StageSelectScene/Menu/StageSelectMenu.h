/*
*	@file	StageSelectMenu.h
*	@brief	���U���g���j���[�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
// DirectX
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <Mouse.h>
// �O�����C�u����
#include "Libraries/MyLib/InputManager.h"
// ����w�b�_�[�t�@�C��
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class StageSelectMenu : public IMenuUI
{
public:// �񋓌^
	enum SceneID// �V�[��ID
	{
		PLAY = 0,// �v���C�V�[��
		END// �^�C�g���ɖ߂�
	};
public:// �A�N�Z�T
	SceneID GetSceneNum() const { return m_num; }// �V�[���ԍ��擾
	void SetSceneNum(SceneID num) { m_num = num; }// �V�[���ԍ��ݒ�
	int GetMenuIndex() const { return m_menuIndex; }// ���j���[�C���f�b�N�X�擾
	void SetMenuIndex(int index) { m_menuIndex = index; }// ���j���[�C���f�b�N�X�ݒ�
	bool GetIsHit()const { return m_hit; }// UI�Ƀq�b�g�������ǂ����擾
	void SetIsHit(bool hit) { m_hit = hit; }// UI�Ƀq�b�g�������ǂ����ݒ�
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SE�̉��ʐݒ�
public:// public�֐�
	StageSelectMenu();// �R���X�g���N�^
	~StageSelectMenu();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// ������
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// �X�V(context���玞�Ԃ��擾���čX�V)
	void Render()override;// �`��
	void Add(const wchar_t* path// ���j���[�A�C�e����ǉ�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type);
private:// private�֐�
	void Update(float elapsedTime);// �X�V
private:// private�萔
	static const int INVALID_MENU_INDEX;// �����ȃ��j���[�C���f�b�N�X
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
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