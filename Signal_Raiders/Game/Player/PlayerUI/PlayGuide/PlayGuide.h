/*
*	@file PlayGuide.h
*	@brief �v���C��������N���X
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
#include "Game/Screen.h"
// �O���錾
class CommonResources;
class PlayGuide : public IPlayUI
{
public:// public�֐�
	PlayGuide();// �R���X�g���N�^
	~PlayGuide();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// �������֐�
	void Update(const UpdateContext& context)override;// �X�V�֐�
	void Render()override;// �`��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key// UI�ǉ��֐�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
	void CreatePlayerUI(std::string key// UI�쐬�֐�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:// private�֐�
	void Update();// �X�V�֐�
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �|�C���^�[UI
	std::unique_ptr<PlayerUI> m_pPlayGuide;
	// �E�B���h�E�̕�
	int m_windowWidth;
	// �E�B���h�E�̍���
	int m_windowHeight;
};
