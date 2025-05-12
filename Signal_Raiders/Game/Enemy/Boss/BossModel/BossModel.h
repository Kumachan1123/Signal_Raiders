/*
	@file	BossModel.h
	@brief	�G���f���N���X
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/Boss.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// �N���X�̑O���錾
class Boss;
class CommonResources;
class IState;

class BossModel : public IModel
{
public:
	// public�����o�֐�
	BossModel();// �R���X�g���N�^
	~BossModel()override;// �f�X�g���N�^
	void Initialize(CommonResources* resources)override;// ������
	void SetState(IState::EnemyState State) { m_nowState = State; }// �X�e�[�g�̐ݒ�
	void Render(ID3D11DeviceContext1* context,// �`��
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ����
	std::unique_ptr<DirectX::Model> m_bodyModel;
	// �U�����̊�
	std::unique_ptr<DirectX::Model> m_attackFaceModel;
	// �m�b�N�o�b�N���̊�
	std::unique_ptr<DirectX::Model> m_damageFaceModel;
	// �{���Ԃ̊�
	std::unique_ptr<DirectX::Model> m_angryFaceModel;
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ���݂̃X�e�[�g
	IState::EnemyState m_nowState;
};
#endif //BOSS_MODEL_DEFINED