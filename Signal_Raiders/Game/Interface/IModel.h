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
class IModel
{
public:
	// public�֐�
	virtual ~IModel() = default;	// �f�X�g���N�^
	virtual void Initialize(CommonResources* resources) = 0;	// ������
	virtual	void SetState(IState::EnemyState State) = 0;	// �X�e�[�g�ݒ�
	virtual void Render(ID3D11DeviceContext1* context,	// �`��
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj) = 0;
};
#endif//IMODEL_DEFINED