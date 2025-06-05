/*
*	@file IBossLogic.h
*	@brief �{�X���W�b�N�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// �W�����C�u����
#include <SimpleMath.h>

// �O���錾
class Player;
class CommonResources;
class BulletManager;

// �{�X���W�b�N�̃C���^�[�t�F�[�X�N���X
class IBossLogic
{
public:
	// public�����o�֐�
	// �f�X�g���N�^
	virtual ~IBossLogic() = default;
	// ������
	virtual void Initialize() = 0;
	// ��ԕύX
	virtual void ChangeState() = 0;
	// �`��
	virtual void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// �e�̔��ˈʒu������
	virtual void BulletPositioning() = 0;
	// �e�𐶐�
	virtual	void CreateBullet() = 0;
};
#endif // IBOSSLOGIC_DEFINED