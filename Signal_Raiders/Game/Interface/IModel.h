/*
*	@file IModel.h
*	@brief ���f���̃C���^�[�t�F�[�X�N���X
*/
#pragma once
#ifndef IMODEL_DEFINED
#define IMODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <Model.h>
// ����w�b�_�[�t�@�C��
#include "Game/Interface/IState.h"

// �O���錾
class CommonResources;
class IState;

// ���f���̃C���^�[�t�F�[�X�N���X
class IModel
{
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IModel() = default;
	// ������
	virtual void Initialize(CommonResources* resources) = 0;
	// �X�e�[�g�ݒ�
	virtual	void SetState(IState::EnemyState State) = 0;
	// �`��
	virtual void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj) = 0;
};
#endif//IMODEL_DEFINED