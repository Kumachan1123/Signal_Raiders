#pragma once
#ifndef IBOSS_DEFINED
#define IBOSS_DEFINED
// ����w�b�_�[�t�@�C��
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Boss/BossSheild/BossSheild.h"
class IBoss : public IEnemy
{
public:
	virtual ~IBoss() = default;
	// �A�N�Z�T
	virtual void SetBulletCooldown(float cooldown) = 0;// �V�[���h��W�J������Ɏ��s����
	virtual void SetInitSpecialAttacCooldown(float cooldown) = 0;// ����U���̃N�[���_�E��
	virtual void PlayBarrierSE() = 0;// �o���ASE�Đ�
	virtual DirectX::SimpleMath::Vector3 GetCameraEye()const = 0;// �J�����̈ʒu
	virtual DirectX::SimpleMath::Vector3 GetCameraTarget()const = 0;// �J�����̃^�[�Q�b�g
	virtual DirectX::SimpleMath::Vector3 GetCameraUp()const = 0;// �J�����̏�x�N�g��
};
#endif //IBOSS_DEFINED