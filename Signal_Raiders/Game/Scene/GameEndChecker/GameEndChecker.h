/*
*	@file	GameEndChecker.h
*	@brief	�Q�[���I���O�ɍĊm�F����UI�N���X
*/
#pragma once
#ifndef GAMEENDCHECKER_DEFINED
#define GAMEENDCHECKER_DEFINED
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <Mouse.h>
#include <WICTextureLoader.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// �O���錾
class CommonResources;

// ���j���[�N���X
class GameEndChecker : public IMenuUI
{
public:
	// �\����
	// UI�̕ό`
	struct UITransForm
	{
		DirectX::SimpleMath::Vector2 position;// �ʒu
		DirectX::SimpleMath::Vector2 scale;// �X�P�[��
	};

public:
	// �A�N�Z�T
	// �Q�[������߂邩�ǂ����擾
	bool GetIsEndGame() const { return m_isEndGame; }
	// �Q�[������߂邩�ǂ����ݒ�
	void SetIsEndGame(bool isEndGame) { m_isEndGame = isEndGame; }
	// ���̃N���X�̏������s�����擾
	bool GetIsGameEndCheck() const { return m_isGameEndCheck; }
	// ���̃N���X�̏������s�����ݒ�
	void SetIsGameEndCheck(bool isGameEndCheck) { m_isGameEndCheck = isGameEndCheck; }
	// �X�e�[�W��I�тȂ������ǂ����擾
	bool GetIsStageSelect() const { return m_isStageSelect; }
	// �X�e�[�W��I�тȂ������ǂ����ݒ�
	void SetIsStageSelect(bool isStageSelect) { m_isStageSelect = isStageSelect; }
	// UI�Ƀq�b�g�������ǂ����擾
	bool GetIsHit()const { return m_hit; }
	// UI�Ƀq�b�g�������ǂ����ݒ�
	void SetIsHit(bool hit) { m_hit = hit; }
	// SE�̉��ʐݒ�
	void SetSEVolume(float volume) { m_SEVolume = volume; }
public:
	// public�֐�
	// �R���X�g���N�^
	GameEndChecker();
	// �f�X�g���N�^
	~GameEndChecker();
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
	std::vector<std::unique_ptr<UI>> m_pNonSelectUI;
	// �I�𒆂�UI�ɕ\������摜
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// �I�𒆂�UI�ɕ\������摜�̃p�X
	const wchar_t* m_pSelectTexturePath;
	// �E�B���h�E�̕��E���� 
	int m_windowWidth, m_windowHeight;
	// UI�̍��W�Ȃǂ̏����i�[����z��
	std::vector<UITransForm> m_transforms;
	// �Q�[������߂邩�ǂ���
	bool m_isEndGame;
	// ����
	float m_time;
	// �����������ǂ���
	bool m_hit;
	// SE�̉���
	float m_SEVolume;
	// �Đ��t���O
	bool m_isSEPlay;
	// ���̃N���X�̏������s�����̃t���O
	bool m_isGameEndCheck;
	// �X�e�[�W��I�тȂ������ǂ����̃t���O
	bool m_isStageSelect;
};
#endif // GAMEENDCHECKER_DEFINED_DEFINED