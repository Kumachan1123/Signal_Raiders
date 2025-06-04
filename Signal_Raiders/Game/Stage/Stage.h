/*
*	@file Stage.h
*	@brief �X�e�[�W�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
// �O���錾
class CommonResources;
class Stage
{
public:// public�֐�
	// �R���X�g���N�^
	Stage();
	// �f�X�g���N�^
	~Stage();
	// ������
	void Initialize(CommonResources* resources);
	// �`��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Matrix& world);
public:// �萔
	// �X�e�[�W�̍L��
	static const float STAGE_SIZE;
	// �X�e�[�W�̍���
	static const float STAGE_HEIGHT;
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���f���|�C���^
	DirectX::Model* m_pModel;
};