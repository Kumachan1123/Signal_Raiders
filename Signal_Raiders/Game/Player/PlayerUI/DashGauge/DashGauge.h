/*
*	@file DashGauge.h
*	@brief �_�b�V���Q�[�W�N���X
*/
#pragma once
#ifndef DASHGAUGE_DEFINED
#define DASHGAUGE_DEFINED
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

// �O���錾
class CommonResources;

// �_�b�V���Q�[�W�N���X
class DashGauge : public IPlayUI
{
public:
	// public�֐�
	// �R���X�g���N�^
	DashGauge();
	// �f�X�g���N�^
	~DashGauge();
	// ������
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V
	void Update(const UpdateContext& context)override { Update(context.dashStamina); }
	// �`��
	void Render()override;
	// UI�ǉ�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private�֐�
	// �X�V
	void Update(float dashStamina);
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �_�b�V���A�C�R��
	std::unique_ptr<PlayerUI> m_pDashIcon;
	// �Q�[�W�{��
	std::unique_ptr<PlayerUI> m_pGauge;
	// �Q�[�W�̘g
	std::unique_ptr<PlayerUI> m_pFrame;
	// �Q�[�W�̔w�i
	std::unique_ptr<PlayerUI> m_pBack;
	// �E�B���h�E�̕�
	int m_windowWidth;
	// �E�B���h�E�̍���
	int m_windowHeight;
};
#endif // DASHGAUGE_DEFINED
