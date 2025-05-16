#pragma once
#ifndef IENEMYBULLET_DEFINED
#define IENEMYBULLET_DEFINED
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
// �O���錾
class EnemyBullet;
class IEnemyBullet
{
public:
	// public�֐�
	virtual ~IEnemyBullet() = default;// �f�X�g���N�^
	virtual void Initialize() = 0;// ������
	virtual void Update(float elapsedTime) = 0;//	�X�V
	virtual void SetEnemyBullet(EnemyBullet* pEnemyBullet) = 0;// �G�e�ݒ�
};
#endif		// IENEMYBULLET_DEFINED
