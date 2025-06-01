/*
*	@file BulletGauge.h
*	@brief �e�Q�[�W�N���X
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
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class BulletGauge : public IPlayUI
{
public:// public�֐�
	BulletGauge();// �R���X�g���N�^
	~BulletGauge();// �f�X�g���N�^
	void Initialize(CommonResources* resources, int width, int height)override;// �������֐�
	void Update(const UpdateContext& context)override { Update(context.bulletPoint); }// �X�V�֐�
	void Render()override;// �`��֐�
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key// �ǉ��֐�
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private�֐�
	void Update(float bulletPoint);// �X�V�֐�
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �Q�[�W
	std::unique_ptr<PlayerUI> m_pGauge;
	// �x�[�X�e�N�X�`���̃p�X
	const wchar_t* m_baseTexturePath;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
};
