/*
	@file	VerticalAttackerModel.h
	@brief	�͈͍U���G���f���N���X
*/
#pragma once
#ifndef AREA_ATTACKER_MODEL_DEFINED
#define AREA_ATTACKER_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <unordered_map>
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"
// �O���錾
class VerticalAttacker;
class CommonResources;
class IState;
class VerticalAttackerModel : public IModel
{
public:
	// public�֐�
	VerticalAttackerModel();// �R���X�g���N�^
	~VerticalAttackerModel()override;// �f�X�g���N�^
	void Initialize(CommonResources* resources)override;// ������
	void SetState(IState::EnemyState State)override { m_nowState = State; }// ��Ԑݒ�
	void Render(ID3D11DeviceContext1* context,// �`��
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;
private:
	// private�ϐ�
	CommonResources* m_commonResources;	// ���ʃ��\�[�X
	// ���f��
	std::unique_ptr<DirectX::Model> m_bodyModel;//����
	std::unordered_map<IState::EnemyState, std::unique_ptr<DirectX::Model>> m_faceModelMap;// �\��ɂ���ĕς���̃��f���̃}�b�v
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	IState::EnemyState m_nowState;	// ���݂̃X�e�[�^�X
};
#endif //AREA_ATTACKER_MODEL_DEFINED