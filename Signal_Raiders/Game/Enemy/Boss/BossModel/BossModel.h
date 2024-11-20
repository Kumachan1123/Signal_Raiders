/*
	@file	BossModel.h
	@brief	�G���f���N���X
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class Boss;
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

	std::unique_ptr < DirectX::Model> m_sheildModel;//�V�[���h
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	DirectX::SimpleMath::Vector3 m_sheildSize;
	DirectX::SimpleMath::Vector3 m_sheildPosition;
	DirectX::SimpleMath::Quaternion m_sheildRotation;
	IState::EnemyState m_nowState;
	// �V�[���h�W�J�t���O
	bool m_isSheild;
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