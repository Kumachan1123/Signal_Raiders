/*
*	@file	WifiLoading.h
*	@brief	Wi-Fi���[�f�B���O�N���X
*/
#pragma once
#ifndef WIFI_LOADING_DEFINED
#define WIFI_LOADING_DEFINED
// �W�����C�u����
#include <vector>
#include <memory>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// ����w�b�_�[�t�@�C�� 
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// Wi-Fi���[�f�B���O�N���X
class WifiLoading : public IPlayUI
{
public:
	// public�֐�
	// �R���X�g���N�^
	WifiLoading();
	// �f�X�g���N�^
	~WifiLoading();
	// �������֐�
	void Initialize(CommonResources* resources, int width, int height) override;
	// �X�V�֐�
	void Update(const UpdateContext& data)override { Update(data.elapsedTime); }
	// �`��֐�
	void Render()override;
	// UI�̒ǉ��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private�֐�
	// �X�V�֐�
	void Update(float elapsedTime);
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// Wi-Fi���[�f�B���OUI
	std::unique_ptr<PlayerUI> m_pLoading;
	// ���[�f�B���O�e�L�X�g
	std::unique_ptr<PlayerUI> m_pLoadgingText;
	// �E�B���h�E�̕�
	int m_windowWidth;
	// �E�B���h�E�̍���
	int m_windowHeight;
	// �t���[���̍s�� 
	int m_frameRows;
	// �t���[���̗�
	int m_frameCols;
	// �t���[����
	int m_anim;
	// �A�j���[�V�����X�s�[�h
	float m_animSpeed;
	// �A�j���[�V��������
	float m_animTime;
};
#endif // WIFI_LOADING_DEFINED