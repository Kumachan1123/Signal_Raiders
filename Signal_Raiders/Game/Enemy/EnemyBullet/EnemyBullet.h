/*
*	@file	EnemyBullet.h
*	@brief	�G�̒e�N���X
*/
#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
// �W�����C�u����
#include <memory>
#include <cassert>
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/CommonResources.h"
#include "Game/Particle/Particle.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/NormalBullet/NormalBullet.h"
#include "Game/Enemy/EnemyBullet/SpeedBullet/SpeedBullet.h"
#include "Game/Enemy/EnemyBullet/SpecialBullet/SpecialBullet.h"
#include "Game/Interface/IBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletFactory/EnemyBulletFactory.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletType/EnemyBulletType.h"

// �O���錾
class CommonResources;
class IEnemyBullet;
class NormalBullet;
class SpeedBullet;
class SpecialBullet;

// �G�̒e�N���X
class EnemyBullet : public IBullet
{
public:
	// �A�N�Z�T
	// �u�e�v���E�����擾
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	// �u�e�v���E����ݒ�
	void SetBoundingSphere(DirectX::BoundingSphere sphere) { m_boundingSphere = sphere; }
	//���Ԃ��擾
	float GetTime()const { return m_time; }
	// �e�̎�ނ��擾
	BulletType GetBulletType()const { return m_bulletType; }
	// �e�̎�ނ�ݒ�
	void SetBulletType(BulletType type) { m_bulletType = type; }
	// �e�̍��W���擾
	DirectX::SimpleMath::Vector3 GetPosition()const { return m_position; }
	// �e�̍��W��ݒ�
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// �e�̑��x���擾
	DirectX::SimpleMath::Vector3 GetVelocity()const { return m_velocity; }
	// �e�̑��x��ݒ�
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { m_velocity = vel; }
	// �e�̕������擾
	DirectX::SimpleMath::Vector3 GetDirection()const { return m_direction; }
	// �e�̕�����ݒ�
	void SetDirection(DirectX::SimpleMath::Vector3 dir) { m_direction = dir; }
	// �e�̑傫�����擾	
	float GetSize()const { return m_size; }
	// �e�̑傫����ݒ�
	void SetSize(float size) { m_size = size; }
	// �G�̍��W���擾
	DirectX::SimpleMath::Vector3 GetEnemyPosition()const { return m_enemyPosition; }
	// �G�̍��W��ݒ�
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 pos) { m_enemyPosition = pos; }
	// ���݂̃^�[�Q�b�g�̈ʒu���擾
	DirectX::SimpleMath::Vector3 GetCurrentTarget()const { return m_currentTarget; }
	// ���݂̃^�[�Q�b�g�̈ʒu��ݒ�
	void SetCurrentTarget(DirectX::SimpleMath::Vector3 target) { m_currentTarget = target; }
	// �e�̍��W���擾
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	// �e�̍��W��ݒ�
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// �J�����̈ʒu��ݒ�
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	// �J�����̃^�[�Q�b�g��ݒ�
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	// �J�����̏�x�N�g����ݒ�
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	// ���˂����G�̃|�C���^�[���擾
	IEnemy* GetShooter() const { return m_pShooter; }
	// ���˂����G�̃|�C���^�[��ݒ�
	void SetShooter(IEnemy* enemy) { m_pShooter = enemy; }
	// �e���ˎ��̃^�[�Q�b�g�̈ʒu���擾
	DirectX::SimpleMath::Vector3 GetTarget() const { return m_target; }
	// �e���ˎ��̃^�[�Q�b�g�̈ʒu��ݒ�
	void SetTarget(DirectX::SimpleMath::Vector3 target) { m_target = target; }
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
	// SE���ʂ��擾
	float GetSEVolume() const { return m_seVolume; }
	// SE���ʂ�ݒ�
	void SetSEVolume(float volume) { m_seVolume = volume; }
public:
	// public�֐�
	// �R���X�g���N�^
	EnemyBullet(float size);
	// �f�X�g���N�^
	~EnemyBullet()override;
	// ������
	void Initialize(CommonResources* pCommonResources)override;
	// �X�V
	void Update(float elapsedTime)override;
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// �e�`��
	void RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// ���E���`��
	void RenderBoundingSphere(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �e�̐���
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir, const DirectX::SimpleMath::Vector3& target);
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const;
private:
	// private�֐�
	//�e�̃��[���h�s����쐬
	DirectX::SimpleMath::Matrix BulletWorldMatrix();
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
	CreateShader* m_pCreateShader;
	// �e�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �G�̍��W
	DirectX::SimpleMath::Vector3 m_enemyPosition;
	// �e�̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �e�̑���
	DirectX::SimpleMath::Vector3 m_velocity;
	// �e�̑傫��
	float m_size;
	// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_direction;
	// �e�̈ʒu�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_positionOffSet;
	// �e�̓G�Ƃ̋���
	float m_distance;
	// �e�̃��f��
	DirectX::Model* m_pModel;
	// ��������
	float m_time;
	// �o�ߎ���
	float m_elapsedTime;
	// ���˂����G�̃|�C���^�[
	IEnemy* m_pShooter;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_target;
	// ���݂̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_currentTarget;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// �J�����̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �J�����̏�x�N�g��
	float m_angle;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �e�̎��]
	// ����e�̉�]�p�x
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
	// �e�̎�ޕʃ|�C���^�[
	// �ʏ�e
	std::unique_ptr<NormalBullet> m_pNormalBullet;
	// �����e
	std::unique_ptr<SpeedBullet> m_pSpeedBullet;
	// ����e
	std::unique_ptr<SpecialBullet> m_pSpecialBullet;
	// �e�̎�ނ��Ƃɏ�����ς��邽�߂̃|�C���^�[
	std::unique_ptr<IEnemyBullet> m_pEnemyBullet;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �e�̎��
	BulletType m_bulletType;
	// SE����
	float m_seVolume;
};
#endif //ENEMY_BULLET_DEFINED