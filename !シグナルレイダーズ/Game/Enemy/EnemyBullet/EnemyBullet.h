/*
	@file	EnemyBullet.h
	@brief	�G�̒e�N���X
	�쐬�ҁF���܂�
*/

#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
#include "Game/CommonResources.h"
#include "Game/BulletTrail/BulletTrail.h"

class CommonResources;

class EnemyBullet
{	//�ϐ�
public:
	enum class BulletType
	{
		STRAIGHT,//����
		SPIRAL,//����
		VERTICAL// ����

	};
	BulletType m_bulletType;
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	float m_size;			// �e�̑傫��
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	float m_time;									// ��������
	const int DAMAGE = 10;						// �G�ɗ^����_���[�W
	// �W�I���g���b�N�v���~�e�B�u�e
	std::unique_ptr<DirectX::Model> m_model;
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	DirectX::SimpleMath::Vector3 m_target;
	// �J����
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// �e�̎��]
	float m_angle;
	float m_spiralAngle;
	// ��]�����i�����e�p:+�Ȃ�E���A-�Ȃ獶���j
	int m_rotateDirection;
	// �e�̑��x
	float m_bulletSpeed;
	// �e�̋O��
	std::unique_ptr<BulletTrail> m_bulletTrail;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:
	EnemyBullet(float size);
	~EnemyBullet();
	void Initialize(CommonResources* resources, BulletType type);
	void Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target);
	//Getter
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::SimpleMath::Vector3 GetBulletDirection()const { return m_direction; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	//Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }

	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const
	{
		const float BULLET_LIFETIME = 30.0f;				// ����

		return GetTime() >= BULLET_LIFETIME;
	}
	// �����e�̉�]���������߂�
	void SetRotateDirection(int direction) { m_rotateDirection = direction; }
private:
	void SpiralBullet();//�����e
	void VerticalBullet();//�����e
	void StraightBullet(DirectX::SimpleMath::Vector3& pos);//�����e

};
#endif //ENEMY_BULLET_DEFINED

