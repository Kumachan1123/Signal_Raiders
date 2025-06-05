/*
*	@file	EnemyModel.h
*	@brief	�G���f���N���X
*/
#pragma once
#ifndef ENEMY_MODEL_DEFINED
#define ENEMY_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// �O���錾
class Enemy;
class CommonResources;
class IState;

// �G���f���N���X
class EnemyModel : public IModel
{
public:
	// public�֐�
	// �R���X�g���N�^
	EnemyModel();
	// �f�X�g���N�^
	~EnemyModel()override;
	// ������
	void Initialize(CommonResources* resources)override;
	// ��Ԃ��Z�b�g
	void SetState(IState::EnemyState State)override { m_nowState = State; }
	// �`��
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj)override;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �����f��
	DirectX::Model* m_pBodyModel;
	// �A���e�i���f��
	DirectX::Model* m_pAntennaModel;
	// �胂�f��
	DirectX::Model* m_pHandModel;
	// �\��ɂ���ĕς���̃��f���̃}�b�v
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// �e�p�̃��f��
	DirectX::Model* m_pShadowModel;
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;
};
#endif //ENEMY_MODEL_DEFINED