/*
*	@file PlayerHP.h
*	@brief HP�Q�[�W�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
 // ����w�b�_�[�t�@�C��
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/KumachiLib.h"
class PlayerHP : public IPlayUI
{
public:// �A�N�Z�T
	void SetMaxHP(float maxHP) { m_maxHP = maxHP; }// �ő�HP��ݒ�
public:// public�֐�
	PlayerHP();// �R���X�g���N�^
	~PlayerHP();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// �������֐�
	void Update(const UpdateContext& context)override { Update(context.playerHP); }// �X�V�֐�
	void Render()override;// �`��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// UI�̒ǉ��֐�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private�֐�
	void Update(float playerHP);// HP�X�V�֐�
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// UI�v�f
	std::unique_ptr<PlayerUI> m_pHeartIcon;// �n�[�g�A�C�R��
	std::unique_ptr<PlayerUI> m_pGauge; // HP�Q�[�W
	std::unique_ptr<PlayerUI> m_pFrame; // HP�Q�[�W�̘g
	std::unique_ptr<PlayerUI> m_pBack; // HP�Q�[�W�̔w�i
	// �E�B���h�E�̕�
	int m_windowWidth;
	// �E�B���h�E�̍���
	int m_windowHeight;
	// �ő�HP(�r���ŕϓ�����j
	float m_maxHP;
	// ���݂�HP
	float m_currentHP;
};
