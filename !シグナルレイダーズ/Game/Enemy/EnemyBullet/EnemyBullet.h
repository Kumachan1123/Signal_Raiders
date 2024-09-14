/*
	@file	EnemyBullet.h
	@brief	�G�̒e�N���X
	�쐬�ҁF���܂�
*/

#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
#include "Game/CommonResources.h"

class CommonResources;

class EnemyBullet
{	//�ϐ�
public:
	const float SIZE = 0.1f;
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
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

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// �e�̎��]
	float m_angle;

	// �u�e�v���E�{�b�N�X
	//DirectX::BoundingSphere m_boundingSphere;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:
	EnemyBullet();
	~EnemyBullet();
	void Initialize(CommonResources* resources);
	void Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target);
	//Getter
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	//Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const
	{
		const float BULLET_LIFETIME = 10.0f;				// ����

		return GetTime() >= BULLET_LIFETIME;
	}
	// �G�Ƀ_���[�W��^����
	int Damage()const { return DAMAGE; }
};
#endif //ENEMY_BULLET_DEFINED

