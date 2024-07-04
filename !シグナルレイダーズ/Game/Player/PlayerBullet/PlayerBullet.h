/*
	@file	PlayerBullet.h
	@brief	�v���C���[�R���g���[���[�N���X
	�쐬�ҁF���܂�
*/
#pragma once
//�O���錾
class CommonResources;
#include "Game/CommonResources.h"
class PlayerBullet
{
	//�ϐ�
public:
	const float SIZE = 0.1f;
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	float m_time;									// ��������
	const float BULLET_LIFETIME = .5;				// ����
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

	// �e�̎��]
	float m_angle;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	// �u�e�v���E�{�b�N�X
	//DirectX::BoundingSphere m_boundingSphere;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//�֐�
public:
	PlayerBullet();
	~PlayerBullet();

	void MoveStop();
	void Initialize(CommonResources* resources);
	void Update(DirectX::SimpleMath::Vector3& Direction, float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir);
	//Getter
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	//Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	bool IsExpired() const { return m_time >= BULLET_LIFETIME; }
	// �G�Ƀ_���[�W��^����
	int Damage()const { return DAMAGE; }
};