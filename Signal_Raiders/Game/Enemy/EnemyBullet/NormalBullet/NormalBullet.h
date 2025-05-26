/*
*	@file NormalBullet.h
*	@brief �G�e�N���X(�ʏ�e)
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"

class NormalBullet : public IEnemyBullet
{
public:
	// public�֐�
	NormalBullet();// �R���X�g���N�^
	~NormalBullet();// �f�X�g���N�^
	void Initialize() override;// �������֐�
	void Update(float elapsedTime) override;// �X�V�֐�
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }// �G�e�|�C���^�[���Z�b�g����֐�
	void SetCommonResources(CommonResources* commonResources) override { m_commonResources = commonResources; }// ���ʃ��\�[�X���Z�b�g����֐�
	void SetSEVolume(float volume) override { m_seVolume = volume; } // SE���ʂ��Z�b�g����֐�
private:
	// private�����o�ϐ�
	CommonResources* m_commonResources;				// ���ʃ��\�[�X	
	EnemyBullet* m_pEnemyBullet;					// �G�e�|�C���^�[
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_target;			// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_toPlayer;		// �v���C���[�̕����x�N�g��
	DirectX::BoundingSphere m_boundingSphere;		// �u�e�v���E��
	float m_seVolume;								// SE����

};