/*
*	@file SpecialBullet.h
*	@brief �G�e�N���X(����e)
*/
#pragma once
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
#include "Game/KumachiLib/KumachiLib.h"
class SpecialBullet : public IEnemyBullet
{
public:
	// public�֐�
	SpecialBullet();// �R���X�g���N�^
	~SpecialBullet();// �f�X�g���N�^
	void Initialize() override;// �������֐�
	void Update(float elapsedTime) override;// �X�V�֐�
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }// �G�e�|�C���^�[���Z�b�g����֐�
	void SetCommonResources(CommonResources* commonResources) override { m_pCommonResources = commonResources; }// ���ʃ��\�[�X���Z�b�g����֐�
	void SetSEVolume(float volume) override { m_seVolume = volume; } // SE���ʂ��Z�b�g����֐�
private:
	// private�֐�
	void Expand();		// ��]�e��W�J����
	void Shot();		// ��]�e�𔭎˂���
	void StopExpand();	// ��]�e�̓W�J����߂�
	void ComeBack();	// ��]�e�������̎���ɖ߂�
private:
	// private�����o�ϐ�	
	CommonResources* m_pCommonResources;				// ���ʃ��\�[�X
	EnemyBullet* m_pEnemyBullet;					// �G�e�|�C���^�[
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_target;			// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_currentTarget;	// ���݂̃^�[�Q�b�g�̈ʒu
	DirectX::BoundingSphere m_boundingSphere;		// �u�e�v���E��
	float m_spiralAngle;							// ��]�e�̊p�x
	float m_elapsedTime;							// �o�ߎ���
	float m_time;									// ����
	DirectX::SimpleMath::Vector3 m_basePos;			// �e�̊�ʒu
	float m_rotationSpeed;							// �e�̉�]���x
	float m_distance;								// �e�̓G�Ƃ̋���
	float m_height;									// �e�̍���
	DirectX::SimpleMath::Vector3 m_positionOffSet;	// �e�̈ʒu�I�t�Z�b�g
	float m_seVolume;								// SE����
	bool m_isPlayChargeSE;							// �`���[�WSE�Đ��t���O
	bool m_isPlayShotSE;							// ����SE�Đ��t���O
};