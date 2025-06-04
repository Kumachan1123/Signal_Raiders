/*
	@file	ResultMenu.h
	@brief	���U���g���j���[�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class ResultMenu : public IMenuUI
{
public:// �񋓌^
	// �V�[��ID
	enum SceneID
	{
		REPLAY = 0,// ���v���C
		SELECT_STAGE,// �X�e�[�W�I��
	};
public:// �A�N�Z�T
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
public:// public�֐�
	// �R���X�g���N�^
	ResultMenu();
	// �f�X�g���N�^
	~ResultMenu();
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
private:// private�֐�
	// �X�V
	void Update(float elapsedTime);
private:// private�萔
	// �����ȃ��j���[�C���f�b�N�X
	static const int INVALID_MENU_INDEX;
private:// private�ϐ�
	// ���j���[�A�C�e���̐�
	unsigned int m_menuIndex;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �I���\��UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// �I��s�\��UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// �I���\��UI���I�΂�Ă���Ƃ��ɏo��UI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �V�[��ID
	SceneID m_num;
	// ����
	float m_time;
	// UI�̃q�b�g�t���O
	bool m_hit;
	// SE�̉���
	float m_SEVolume;
	// �Đ��t���O
	bool m_isSEPlay;
};