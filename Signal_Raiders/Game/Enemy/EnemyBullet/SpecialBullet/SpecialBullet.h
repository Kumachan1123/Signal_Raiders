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
public:// public�֐�
	// �R���X�g���N�^
	SpecialBullet();
	// �f�X�g���N�^
	~SpecialBullet();
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
private:// private�֐�
	// ��]�e��W�J����
	void Expand();
	// ��]�e�𔭎˂���
	void Shot();
	// ��]�e�̓W�J����߂�
	void StopExpand();
	// ��]�e�������̎���ɖ߂�
	void ComeBack();
private:// private�����o�ϐ�	
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
	// ���݂̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_currentTarget;
	// �u�e�v���E��
	DirectX::BoundingSphere m_boundingSphere;
	// ��]�e�̊p�x
	float m_spiralAngle;
	// �o�ߎ���
	float m_elapsedTime;
	// ����
	float m_time;
	// �e�̊�ʒu
	DirectX::SimpleMath::Vector3 m_basePos;
	// �e�̉�]���x
	float m_rotationSpeed;
	// �e�̓G�Ƃ̋���
	float m_distance;
	// �e�̍���
	float m_height;
	// �e�̈ʒu�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_positionOffSet;
	// SE����
	float m_seVolume;
	// �`���[�WSE�Đ��t���O
	bool m_isPlayChargeSE;
	// ����SE�Đ��t���O
	bool m_isPlayShotSE;
};