/*
	@file	CowardlyEnemyModel.h
	@brief	���΂��������ڋ��ȓG���f���N���X
*/
#pragma once
#ifndef  COWARDLY_ENEMY_MODEL_DEFINED
#define  COWARDLY_ENEMY_MODEL_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
#include <unordered_map>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/Enemy/CowardlyEnemy/CowardlyEnemy.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// �O���錾
class VerticalAttacker;
class CommonResources;
class IState;

// ���΂��������Ĕڋ��ȓG���f���N���X
class CowardlyEnemyModel : public IModel
{
public:
	// public�֐�
	// �R���X�g���N�^
	CowardlyEnemyModel();
	// �f�X�g���N�^
	~CowardlyEnemyModel()override;
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
	// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
	CreateShader* m_pCreateShader;
	// ���̃��f��
	DirectX::Model* m_pBodyModel;
	// �\��ɂ���ĕς���̃��f���̃}�b�v
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// ���f���̉e�p�̃s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ���݂̃X�e�[�^�X
	IState::EnemyState m_nowState;
};
#endif // COWARDLY_ENEMY_MODEL_DEFINED