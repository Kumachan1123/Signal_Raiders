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
class IPlayUI : public IUI
{
public:
	virtual ~IPlayUI() = default;// �f�X�g���N�^
	virtual void Add// UI�ǉ�
	(std::unique_ptr<PlayerUI>& pPlayerUI// �v���C���[UI
		, std::string key// �e�N�X�`���p�X
		, DirectX::SimpleMath::Vector2 position// �ʒu
		, DirectX::SimpleMath::Vector2 scale// �X�P�[��
		, KumachiLib::ANCHOR anchor) = 0;// �A���J�[
};
#endif		// IPLAYUI_DEFINED
