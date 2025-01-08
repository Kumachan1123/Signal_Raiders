/*
	@file	EnemyModel.h
	@brief	�G���f���N���X
*/
#pragma once
#ifndef ENEMY_MODEL_DEFINED
#define ENEMY_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class Enemy;
class CommonResources;
class IState;
class EnemyModel
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���f��
	std::unique_ptr<DirectX::Model> m_bodyModel;//����
	std::unique_ptr<DirectX::Model> m_antennaModel;//�A���e�i
	std::unique_ptr<DirectX::Model> m_handModel;//��
	std::unique_ptr<DirectX::Model> m_attackFaceModel;//�U���Ԑ��̊�
	std::unique_ptr<DirectX::Model> m_angryFaceModel;//�����̎��̊�
	std::unique_ptr<DirectX::Model> m_idlingFaceModel;//���i�̊�
	std::unique_ptr<DirectX::Model> m_damageFaceModel;	// �U�����󂯂����̊�
	std::unique_ptr<DirectX::Model> m_shadowModel;	// �e�p�̃��f��
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;
public:
	// �����X�e�[�^�X��ݒ�
	EnemyModel();
	~EnemyModel();
	void Initialize(CommonResources* resources);
	void SetState(IState::EnemyState State) { m_nowState = State; }
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);

};
#endif //ENEMY_MODEL_DEFINED