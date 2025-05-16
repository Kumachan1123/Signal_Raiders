#pragma once
#ifndef IMENUUI_DEFINED
#define IMENUUI_DEFINED
// �W�����C�u����
#include <memory>
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/Anchor.h"
#include "Game/Interface/IUI.h"
// �O���錾
class CommonResources;
class IMenuUI : public IUI
{
public:
	// �񋓌^
	enum class UIType//UI�̎��
	{
		SELECT,		// �I���\UI
		NON_SELECT	// �I��s�\UI
	};
public:
	virtual ~IMenuUI() = default;// �f�X�g���N�^
	virtual void Add(							// UI�ǉ�	
		const wchar_t* path						// �e�N�X�`���p�X
		, DirectX::SimpleMath::Vector2 position	// �ʒu
		, DirectX::SimpleMath::Vector2 scale	// �X�P�[��
		, KumachiLib::ANCHOR anchor				// �A���J�[
		, IMenuUI::UIType type) = 0;			// UI�̎��	
};
#endif		// IMENUUI_DEFINED
