/*
	@file	AreaAttackerModel.h
	@brief	�͈͍U���G���f���N���X
	�쐬�ҁF���܂�
*/
#pragma once
#ifndef AREA_ATTACKER_MODEL_DEFINED
#define AREA_ATTACKER_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class AreaAttacker;
class CommonResources;
class IState;
class AreaAttackerModel
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���f��
	std::unique_ptr<DirectX::Model> m_bodyModel;//����
	std::unique_ptr<DirectX::Model> m_attackFaceModel;//�U���Ԑ��̊�
	std::unique_ptr<DirectX::Model> m_angryFaceModel;//�����̎��̊�
	std::unique_ptr<DirectX::Model> m_idlingFaceModel;//���i�̊�
	std::unique_ptr<DirectX::Model> m_damageFaceModel;	// �U�����󂯂����̊�
	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;
public:
	// �����X�e�[�^�X��ݒ�
	AreaAttackerModel();
	~AreaAttackerModel();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, IState::EnemyState State);
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
};
#endif //AREA_ATTACKER_MODEL_DEFINED