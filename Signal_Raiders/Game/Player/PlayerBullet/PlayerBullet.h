/*
*	@file	PlayerBullet.h
*	@brief	�v���C���[�̒e�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX���C�u����
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib//DrawCollision/DrawCollision.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Particle/Particle.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Interface/IBullet.h"
//�O���錾
class CommonResources;

class PlayerBullet : public IBullet
{
public:// �A�N�Z�T
	// �e�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	// �e�̍��W��ݒ肷��
	void SetBulletPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// �e�̋��E�����擾����
	const DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	// �v���C���[��Y���W���擾����
	float GetPlayerPositionY()const { return m_position.y; }
	// ���Ԃ��擾����
	float GetTime()const { return m_time; }
	// �J�����̎��_��ݒ肷��
	void SetCameraEye(const DirectX::SimpleMath::Vector3& eye) { m_pBulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	// �J�����̒����_��ݒ肷��
	void SetCameraTarget(const DirectX::SimpleMath::Vector3& target) { m_pBulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	// �J�����̏������ݒ肷��
	void SetCameraUp(const DirectX::SimpleMath::Vector3& up) { m_pBulletTrail->SetCameraUp(up); m_cameraUp = up; }
	// �ǉ��_���[�W���擾����
	int GetAdditionalDamage()const { return m_additionalDamage; }
	// �ǉ��_���[�W��ݒ肷��
	void SetAdditionalDamage(int damage) { m_additionalDamage = damage; }
public:// public�֐�
	// �R���X�g���N�^
	PlayerBullet();
	// �f�X�g���N�^
	~PlayerBullet()override;
	// ������
	void Initialize(CommonResources* resources)override;
	// �X�V
	void Update(float elapsedTime)override;
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// �e�̕`��
	void RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// �����蔻��̕`��
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �e�𐶐�
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const { return m_time >= BulletParameters::PLAYER_BULLET_LIFETIME; }
	// �G�Ƀ_���[�W��^����
	int Damage()const { return BulletParameters::DAMAGE + m_additionalDamage; }
private:// private�ϐ�
	// �O�՗p�̗�����ۑ�����z��
	std::vector<DirectX::SimpleMath::Vector3> m_trailPositions;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �e�̎��]
	float m_angle;
	// �e�̋O�Ճ|�C���^�[
	std::unique_ptr<Particle> m_pBulletTrail;
	// �ǉ��_���[�W
	int m_additionalDamage;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// �J�����̌���
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �J�����̏����
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �e�̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �e�̑���
	DirectX::SimpleMath::Vector3 m_velocity;
	// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_direction;
	// ��������
	float m_time;
	// �e���f��
	DirectX::Model* m_pModel;
};