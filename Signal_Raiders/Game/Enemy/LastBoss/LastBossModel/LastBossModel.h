/*
*	@file	LastBossModel.h
*	@brief	���X�{�X���f���N���X
*/
#pragma once
#ifndef LASTBOSS_MODEL_DEFINED
#define LASTBOSS_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// �N���X�̑O���錾
class Boss;
class CommonResources;
class IState;

// ���X�{�X���f���N���X
class LastBossModel : public IModel
{
public:
	// public�����o�֐�
	// �R���X�g���N�^
	LastBossModel();
	// �f�X�g���N�^
	~LastBossModel()override;
	// ������
	void Initialize(CommonResources* resources)override;
	// �X�e�[�g�̐ݒ�
	void SetState(IState::EnemyState State) { m_nowState = State; }
	// �`��
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj)	override;
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
	CreateShader* m_pCreateShader;
	// ����
	DirectX::Model* m_pBodyModel;
	// �\��ɂ���ĕς���̃��f���̃}�b�v
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ���݂̃X�e�[�g
	IState::EnemyState m_nowState;
};
#endif //LASTBOSS_MODEL_DEFINED