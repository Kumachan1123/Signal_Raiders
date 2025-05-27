/*
*	@file Reticle.h
*	@brief �Ə��N���X
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
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/Screen.h"
#include "Game/Interface/IPlayUI.h"
class Reticle : public IPlayUI
{
public:// public�֐�
	Reticle();// �R���X�g���N�^
	~Reticle();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// �������֐�
	void Update(const UpdateContext& context)override;// �X�V�֐�
	void Render()override;// �`��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// UI�̒ǉ��֐�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private�֐�
	void Update();// �X�V�֐�
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �Ə�UI
	std::unique_ptr<PlayerUI> m_pReticle;
	// �E�B���h�E�̕�
	int m_windowWidth;
	// �E�B���h�E�̍���
	int m_windowHeight;

};
