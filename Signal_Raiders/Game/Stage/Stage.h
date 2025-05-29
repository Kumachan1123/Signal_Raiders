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
	Stage();// �R���X�g���N�^
	~Stage();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void Render(DirectX::SimpleMath::Matrix view,// �`��
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
public:// �萔
	static const float STAGE_SIZE;	// �X�e�[�W�̍L��
	static const float STAGE_HEIGHT;	// �X�e�[�W�̍���
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���f���|�C���^
	DirectX::Model* m_pModel;
};