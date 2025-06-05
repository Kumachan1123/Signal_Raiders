/*
*	@file IPlayUI.h
*	@brief �v���CUI�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
#ifndef IPLAYUI_DEFINED
#define IPLAYUI_DEFINED
// �W�����C�u����
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IUI.h"

// �O���錾
class CommonResources;

// �C���^�[�t�F�[�X�N���X�F�v���CUI
class IPlayUI : public IUI
{
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IPlayUI() = default;
	// UI�ǉ�
	virtual void Add(
		std::unique_ptr<PlayerUI>& pPlayerUI// �v���C���[UI
		, const std::string& key// �e�N�X�`���p�X
		, const DirectX::SimpleMath::Vector2& position// �ʒu
		, const DirectX::SimpleMath::Vector2& scale// �X�P�[��
		, KumachiLib::ANCHOR anchor) = 0;// �A���J�[
};
#endif		// IPLAYUI_DEFINED
