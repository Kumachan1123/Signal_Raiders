/*
*	@file DashGauge.h
*	@brief �_�b�V���Q�[�W�N���X
*/
#pragma once
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
class DashGauge : public IPlayUI
{
public:// public�֐�
	DashGauge();// �R���X�g���N�^
	~DashGauge();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// ������
	void Update(const UpdateContext& context)override { Update(context.dashStamina); }// �X�V
	void Render()override;// �`��
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key// UI�ǉ�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private�֐�
	void Update(float dashStamina);// �X�V
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
