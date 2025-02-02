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
		SPIRAL,//����
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
	std::unique_ptr<DirectX::Model> m_model;
	float m_time;									// ��������
	// ���˂����G�̃|�C���^�[
	IEnemy* m_pShooter;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	// �^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_target;
	// �J����
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �e�̎��]
	float m_angle;
	float m_spiralAngle;
	// ��]�����i�����e�p:+�Ȃ�E���A-�Ȃ獶���j
	int m_rotateDirection;
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
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::SimpleMath::Vector3 GetBulletDirection()const { return m_direction; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }

	BulletType GetBulletType()const { return m_bulletType; }// �e�̎�ނ��擾

	//Setter
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 pos) { m_enemyPosition = pos; }
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	void SetShooter(IEnemy* enemy) { m_pShooter = enemy; }
	IEnemy* GetShooter() const { return m_pShooter; }
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const { return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME; }

	// �����e�̉�]���������߂�
	void SetRotateDirection(int direction) { m_rotateDirection = direction; }
private:
	void SpiralBullet();//�����e
	void VerticalBullet();//�������i�e
	void StraightBullet();//�����e
	DirectX::SimpleMath::Matrix BulletWorldMatrix();//�e�̃��[���h�s����쐬
};
#endif //ENEMY_BULLET_DEFINED

