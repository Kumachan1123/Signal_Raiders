/*
	@file	BossModel.h
	@brief	�G���f���N���X
	�쐬�ҁF���܂�
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class Enemy;
class CommonResources;
class IState;
class BossModel
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���f��
	std::unique_ptr<DirectX::Model> m_bodyModel;//����

	std::unique_ptr<DirectX::Model> m_attackFaceModel;//�U�����̊�
	std::unique_ptr<DirectX::Model> m_idlingFaceModel;//���i�̊�

	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Vector3 m_accele;		// �����x
	DirectX::SimpleMath::Vector3 m_nowScale;		// ���݃X�P�[��
	DirectX::SimpleMath::Vector3 m_startScale;		// �����X�P�[��
	DirectX::SimpleMath::Vector3 m_endScale;		// �ŏI�X�P�[��
	DirectX::SimpleMath::Quaternion m_rotation; // �N�H�[�^�j�I�� (�ǉ�)
	IState::EnemyState m_nowState;
public:
	// �����X�e�[�^�X��ݒ�
	BossModel();
	~BossModel();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, IState::EnemyState State);
	void Render(ID3D11DeviceContext1* context,
				DirectX::DX11::CommonStates* states,
				DirectX::SimpleMath::Matrix world,
				DirectX::SimpleMath::Matrix view,
				DirectX::SimpleMath::Matrix proj);

};
#endif //BOSS_MODEL_DEFINED