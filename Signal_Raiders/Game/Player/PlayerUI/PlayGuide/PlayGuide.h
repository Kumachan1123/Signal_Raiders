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
	// �R���X�g���N�^
	PlayGuide();
	// �f�X�g���N�^
	~PlayGuide();
	// �������֐�
	void Initialize(CommonResources* resources, int width, int height)override;
	// �X�V�֐�
	void Update(const UpdateContext& context)override;
	// �`��֐�
	void Render()override;
	// UI�ǉ��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
	// UI�쐬�֐�
	void CreatePlayerUI(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor);
private:// private�֐�
	// �X�V�֐�
	void Update();
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
