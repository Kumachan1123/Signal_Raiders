/*
*	@file	BossModel.h
*	@brief	�G���f���N���X
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
// �W�����C�u����
#include <unordered_map>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"
// �O���錾
class Boss;
class CommonResources;
class IState;
class BossModel : public IModel
{
public:// public�����o�֐�
	// �R���X�g���N�^
	BossModel();
	// �f�X�g���N�^
	~BossModel()override;
	// ������
	void Initialize(CommonResources* resources)override;
	// �X�e�[�g�̐ݒ�
	void SetState(IState::EnemyState State) { m_nowState = State; }
	// �`��
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj)override;
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ����
	DirectX::Model* m_pBodyModel;
	// �\��ɂ���ĕς���̃��f���̃}�b�v
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ���݂̃X�e�[�g
	IState::EnemyState m_nowState;
};
#endif //BOSS_MODEL_DEFINED