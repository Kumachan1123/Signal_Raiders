/*
*	@file PlayerHP.h
*	@brief HP�Q�[�W�N���X
*/
#pragma once
#ifndef PLAYER_HP_DEFINED
#define PLAYER_HP_DEFINED
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
 // ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/KumachiLib.h"

// �v���C���[��HP�Q�[�W�N���X
class PlayerHP : public IPlayUI
{
public:
	// �A�N�Z�T
	// �ő�HP��ݒ�
	void SetMaxHP(float maxHP) { m_maxHP = maxHP; }
public:
	// public�֐�
	// �R���X�g���N�^
	PlayerHP();
	// �f�X�g���N�^
	~PlayerHP();
	// �������֐�
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V�֐�
	void Update(const UpdateContext& context)override { Update(context.playerHP); }
	// �`��֐�
	void Render()override;
	// UI�̒ǉ��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private�֐�
	// HP�X�V�֐�
	void Update(float playerHP);
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// UI�v�f
	// �n�[�g�A�C�R��
	std::unique_ptr<PlayerUI> m_pHeartIcon;
	// HP�Q�[�W
	std::unique_ptr<PlayerUI> m_pGauge;
	// HP�Q�[�W�̘g
	std::unique_ptr<PlayerUI> m_pFrame;
	// HP�Q�[�W�̔w�i
	std::unique_ptr<PlayerUI> m_pBack;
	// �E�B���h�E�̕�
	int m_windowWidth;
	// �E�B���h�E�̍���
	int m_windowHeight;
	// �ő�HP(�r���ŕϓ�����j
	float m_maxHP;
	// ���݂�HP
	float m_currentHP;
};
#endif // PLAYER_HP_DEFINED