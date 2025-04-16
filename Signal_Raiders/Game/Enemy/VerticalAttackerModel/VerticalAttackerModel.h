/*
	@file	VerticalAttackerModel.h
	@brief	�͈͍U���G���f���N���X
*/
#pragma once
#ifndef AREA_ATTACKER_MODEL_DEFINED
#define AREA_ATTACKER_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"
class VerticalAttacker;
class CommonResources;
class IState;
class VerticalAttackerModel : public IModel
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
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;
public:
	// �����X�e�[�^�X��ݒ�
	VerticalAttackerModel();
	~VerticalAttackerModel()override;
	void Initialize(CommonResources* resources)override;
	void SetState(IState::EnemyState State)override { m_nowState = State; }
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;
};
#endif //AREA_ATTACKER_MODEL_DEFINED