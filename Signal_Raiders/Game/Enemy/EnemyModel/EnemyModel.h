/*
	@file	EnemyModel.h
	@brief	�G���f���N���X
*/
#pragma once
#ifndef ENEMY_MODEL_DEFINED
#define ENEMY_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <unordered_map>
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"
// �O���錾
class Enemy;
class CommonResources;
class IState;
class EnemyModel : public IModel
{
public:
	// public�֐�
	EnemyModel();// �R���X�g���N�^
	~EnemyModel()override;	// �f�X�g���N�^
	void Initialize(CommonResources* resources)override;//�@������
	void SetState(IState::EnemyState State)override { m_nowState = State; }// ��Ԃ��Z�b�g
	void Render(ID3D11DeviceContext1* context,// �`��
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���f��
	DirectX::Model* m_bodyModel;// �� 
	DirectX::Model* m_antennaModel;//�A���e�i
	DirectX::Model* m_handModel;//��
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;// �\��ɂ���ĕς���̃��f���̃}�b�v
	DirectX::Model* m_shadowModel;	// �e�p�̃��f��
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;

};
#endif //ENEMY_MODEL_DEFINED