/*
	@file	PlayerBullet.h
	@brief	�v���C���[�̒e�N���X
*/
#pragma once
//�O���錾
class CommonResources;
#include "Game/CommonResources.h"
#include "Game/Particle/Particle.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include "Game/BulletParameters/BulletParameters.h"
class PlayerBullet
{
	//�ϐ�
public:

	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	float m_time;									// ��������
	// �W�I���g���b�N�v���~�e�B�u�e
	std::unique_ptr<DirectX::Model> m_model;
private:
	// �O�՗p�̗�����ۑ�����z��
	std::vector<DirectX::SimpleMath::Vector3> m_trailPositions;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// �e�̎��]
	float m_angle;
	// �e�̋O�Ճ|�C���^�[
	std::unique_ptr<Particle> m_bulletTrail;
	// �ǉ��_���[�W
	int m_additionalDamage;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_trailTexture;
	// �J����
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	//�֐�
public:
	PlayerBullet();
	~PlayerBullet();

	void MoveStop();// �e�̈ړ����~�߂�
	void Initialize(CommonResources* resources);// ������
	void Update(float elapsedTime);// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �`��
	void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �e�̕`��
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �����蔻��̕`��
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir);// �e�𐶐�
	//Getter
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	//Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const { return m_time >= BulletParameters::PLAYER_BULLET_LIFETIME; }
	// �G�Ƀ_���[�W��^����
	int Damage()const { return BulletParameters::DAMAGE + m_additionalDamage; }
	void SetAdditionalDamage(int damage) { m_additionalDamage = damage; }
	int GetAdditionalDamage()const { return m_additionalDamage; }
};