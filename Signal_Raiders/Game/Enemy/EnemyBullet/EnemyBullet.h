/*
	@file	EnemyBullet.h
	@brief	�G�̒e�N���X
*/

#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
#include <memory>
#include "Game/CommonResources.h"
#include "Game/Particle/Particle.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/NormalBullet/NormalBullet.h"
#include "Game/Enemy/EnemyBullet/SpeedBullet/SpeedBullet.h"
#include "Game/Enemy/EnemyBullet/SpecialBullet/SpecialBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Interface/IBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletFactory/EnemyBulletFactory.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletType/EnemyBulletType.h"
class CommonResources;
class IEnemyBullet;
class NormalBullet;
class SpeedBullet;
class SpecialBullet;
class EnemyBullet : public IBullet
{	//�ϐ�
public:
	//enum class BulletType
	//{
	//	NORMAL,//����
	//	SPECIAL,//����
	//	SPEED// ����

	//};
	BulletType m_bulletType;// �e�̎��
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;		// �e�̃��[���h�s��
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
	// ��]���x
	float m_rotationSpeed;
	// �e�̑��x
	float m_bulletSpeed;
	// �e�̋O��
	std::unique_ptr<Particle> m_bulletTrail;
	std::unique_ptr<NormalBullet> m_pNormalBullet;
	std::unique_ptr<SpeedBullet> m_pSpeedBullet;
	std::unique_ptr<SpecialBullet> m_pSpecialBullet;
	// �e�̎�ނ��Ƃɏ�����ς��邽�߂̃|�C���^�[
	std::unique_ptr<IEnemyBullet> m_pEnemyBullet;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
public:
	EnemyBullet(float size);
	~EnemyBullet()override;
	void Initialize(CommonResources* resources)override;
	void Update(float elapsedTime)override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;
	void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;
	void RenderBoundingSphere(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target);
	//Getter

	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }// �u�e�v���E�����擾
	void SetBoundingSphere(DirectX::BoundingSphere sphere) { m_boundingSphere = sphere; }// �u�e�v���E����ݒ�
	float GetTime()const { return m_time; }
	BulletType GetBulletType()const { return m_bulletType; }// �e�̎�ނ��擾
	void SetBulletType(BulletType type) { m_bulletType = type; }// �e�̎�ނ�ݒ�
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }// �e�̍��W���擾
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �e�̍��W��ݒ�
	DirectX::SimpleMath::Vector3 GetVelocity()const { return m_velocity; }// �e�̑��x���擾
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { m_velocity = vel; }// �e�̑��x��ݒ�
	DirectX::SimpleMath::Vector3 GetDirection()const { return m_direction; }// �e�̕������擾
	void SetDirection(DirectX::SimpleMath::Vector3 dir) { m_direction = dir; }// �e�̕�����ݒ�
	float GetSize()const { return m_size; }// �e�̑傫�����擾	
	void SetSize(float size) { m_size = size; }// �e�̑傫����ݒ�
	DirectX::SimpleMath::Vector3 GetEnemyPosition()const { return m_enemyPosition; }// �G�̍��W���擾
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 pos) { m_enemyPosition = pos; }// �G�̍��W��ݒ�
	DirectX::SimpleMath::Vector3 GetCurrentTarget()const { return m_currentTarget; }// ���݂̃^�[�Q�b�g�̈ʒu���擾
	void SetCurrentTarget(DirectX::SimpleMath::Vector3 target) { m_currentTarget = target; }// ���݂̃^�[�Q�b�g�̈ʒu��ݒ�
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }// �e�̍��W���擾
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �e�̍��W��ݒ�
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	IEnemy* GetShooter() const { return m_pShooter; }
	void SetShooter(IEnemy* enemy) { m_pShooter = enemy; }
	DirectX::SimpleMath::Vector3 GetTarget() const { return m_target; }
	void SetTarget(DirectX::SimpleMath::Vector3 target) { m_target = target; }
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

private:

	DirectX::SimpleMath::Matrix BulletWorldMatrix();//�e�̃��[���h�s����쐬
};
#endif //ENEMY_BULLET_DEFINED

