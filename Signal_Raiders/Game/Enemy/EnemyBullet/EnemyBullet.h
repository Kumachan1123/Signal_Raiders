/*
	@file	EnemyBullet.h
	@brief	�G�̒e�N���X
*/

#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
#include "Game/CommonResources.h"
#include "Game/Particle/Particle.h"
#include "Game/Interface/IEnemy.h"
#include "Game/BulletParameters/BulletParameters.h"
class CommonResources;
class EnemyBullet
{	//�ϐ�
public:
	enum class BulletType
	{
		STRAIGHT,//����
		SPIRAL,//��]
		VERTICAL// ����

	};
	BulletType m_bulletType;// �e�̎��
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	DirectX::SimpleMath::Vector3 m_enemyPosition;	// �G�̍��W
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	float m_size;									// �e�̑傫��
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_positionOffSet;	// �e�̈ʒu�I�t�Z�b�g
	float m_distance;								// �e�̓G�Ƃ̋���
	std::unique_ptr<DirectX::Model> m_model;
	float m_time;									// ��������
	float m_elapsedTime;							// �o�ߎ���
	// ���˂����G�̃|�C���^�[
	IEnemy* m_pShooter;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_target;
	// ���݂̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_currentTarget;
	// �J����
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �e�̎��]
	float m_angle;
	float m_spiralAngle;
	// ��]�����i�����e�p:+�Ȃ�E���A-�Ȃ獶���j
	int m_rotateDirection;
	// ����
	float m_height;
	// �p�x
	// �W�J�����t���O
	bool m_isExpand;
	// ���˃t���O
	bool m_isShot;
	// ��]��n�_
	DirectX::SimpleMath::Vector3 m_basePos;
	// �ړ������_�̖ړI�n
	const DirectX::SimpleMath::Vector3 m_targetPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -30.0f);
	// ��]���x
	float m_rotationSpeed;
	// �e�̑��x
	float m_bulletSpeed;
	// �e�̋O��
	std::unique_ptr<Particle> m_bulletTrail;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
public:
	EnemyBullet(float size);
	~EnemyBullet();
	void Initialize(CommonResources* resources, BulletType type);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void RenderBoundingSphere(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target);
	//Getter

	DirectX::SimpleMath::Vector3 GetBulletDirection()const { return m_direction; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetTime()const { return m_time; }
	BulletType GetBulletType()const { return m_bulletType; }// �e�̎�ނ��擾
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 pos) { m_enemyPosition = pos; }// �G�̍��W��ݒ�
	void SetCurrentTarget(DirectX::SimpleMath::Vector3 target) { m_currentTarget = target; }// ���݂̃^�[�Q�b�g�̈ʒu��ݒ�
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }// �e�̍��W���擾
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �e�̍��W��ݒ�
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	IEnemy* GetShooter() const { return m_pShooter; }
	void SetShooter(IEnemy* enemy) { m_pShooter = enemy; }
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const;// { return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME; }
	// �v���C���[����̋������擾
	float GetDistance() const { return m_distance; }
	// �v���C���[����̋�����ݒ�
	void SetDistance(float distance) { m_distance = distance; }
	// �v���C���[�̊p�x���擾
	float GetAngle() const { return m_spiralAngle; }
	// �v���C���[�̊p�x��ݒ�
	void SetAngle(float angle) { m_spiralAngle = angle; }
	// �W�J�����t���O���擾
	bool GetIsExpand() const { return m_isExpand; }
	// �W�J�����t���O��ݒ�
	void SetIsExpand(bool isExpand) { m_isExpand = isExpand; }
	// ���˃t���O���擾
	bool GetIsShot() const { return m_isShot; }
	// ���˃t���O��ݒ�
	void SetIsShot(bool isShot) { m_isShot = isShot; }
	// �����e�̉�]���������߂�
	void SetRotateDirection(int direction) { m_rotateDirection = direction; }
	// ��]�e��W�J����
	void Expand();
	// ��]�e�𔭎˂���
	void Shot();
	// ��]�e�̓W�J����߂�
	void StopExpand();
	// ��]�e�������̎���ɖ߂�
	void ComeBack();


private:
	void SpiralBullet(float elapsedTime);//�����e
	void VerticalBullet(float elapsedTime);//�������i�e
	void StraightBullet(float elapsedTime);//�����e
	DirectX::SimpleMath::Matrix BulletWorldMatrix();//�e�̃��[���h�s����쐬
};
#endif //ENEMY_BULLET_DEFINED

