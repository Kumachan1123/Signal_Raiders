/*
*	@file NormalBullet.h
*	@brief �G�e�N���X(�ʏ�e)
*/
#pragma once
#ifndef NORMAL_BULLET_DEFINED
#define NORMAL_BULLET_DEFINED

// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"

//  �G�e�N���X(�ʏ�e)
class NormalBullet : public IEnemyBullet
{
public:
	// public�֐�
	// �R���X�g���N�^
	NormalBullet();
	// �f�X�g���N�^
	~NormalBullet();
	// �������֐�
	void Initialize() override;
	// �X�V�֐�
	void Update(float elapsedTime) override;
	// �G�e�|�C���^�[���Z�b�g����֐�
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
	// ���ʃ��\�[�X���Z�b�g����֐�
	void SetCommonResources(CommonResources* commonResources) override { m_pCommonResources = commonResources; }
	// SE���ʂ��Z�b�g����֐�
	void SetSEVolume(float volume) override { m_seVolume = volume; }
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X	
	CommonResources* m_pCommonResources;
	// �G�e�|�C���^�[
	EnemyBullet* m_pEnemyBullet;
	// �e�̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �e�̑���
	DirectX::SimpleMath::Vector3 m_velocity;
	// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_direction;
	// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_target;
	// �v���C���[�̕����x�N�g��
	DirectX::SimpleMath::Vector3 m_toPlayer;
	// �u�e�v���E��
	DirectX::BoundingSphere m_boundingSphere;
	// SE����
	float m_seVolume;
};
#endif // NORMAL_BULLET_DEFINED