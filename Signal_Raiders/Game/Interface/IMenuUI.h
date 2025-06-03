#pragma once
#ifndef IMENUUI_DEFINED
#define IMENUUI_DEFINED
// �W�����C�u����
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/Anchor.h"
#include "Game/Interface/IUI.h"
// �O���錾
class CommonResources;
class IMenuUI : public IUI
{
public:	// �񋓌^
	//UI�̎��
	enum class UIType
	{
		SELECT,		// �I���\UI
		NON_SELECT	// �I��s�\UI
	};
public:// public�֐�
	// �f�X�g���N�^
	virtual ~IMenuUI() = default;
	// UI�ǉ�	
	virtual void Add(
		const std::string& key					// �e�N�X�`���̃L�[
		, const DirectX::SimpleMath::Vector2& position	// �ʒu
		, const DirectX::SimpleMath::Vector2& scale	// �X�P�[��
		, KumachiLib::ANCHOR anchor				// �A���J�[
		, IMenuUI::UIType type) = 0;			// UI�̎��	
};
#endif		// IMENUUI_DEFINED
