/*
	@file	VerticalAttackerModel.h
	@brief	�����U���G���f���N���X
*/
#pragma once
#ifndef VERTICAL_ATTACKER_MODEL_DEFINED
#define VERTICAL_ATTACKER_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <unordered_map>
// �O�����C�u����
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// �O���錾
class VerticalAttacker;
class CommonResources;
class IState;

// �����U���G���f���N���X
class VerticalAttackerModel : public IModel
{
public:
	// public�֐�
	// �R���X�g���N�^
	VerticalAttackerModel();
	// �f�X�g���N�^
	~VerticalAttackerModel()override;
	// ������
	void Initialize(CommonResources* resources)override;
	// ��Ԑݒ�
	void SetState(IState::EnemyState State)override { m_nowState = State; }
	// �`��
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj)	override;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���̃��f��
	DirectX::Model* m_pBodyModel;
	// �\��ɂ���ĕς���̃��f���̃}�b�v
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;
};
#endif //VERTICAL_ATTACKER_MODEL_DEFINED