/*
*	@file IUI.h
*	@brief UI�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
#ifndef IUI_DEFINED
#define IUI_DEFINED
// �W�����C�u����
#include <SimpleMath.h>

// �O���錾
class CommonResources;

// UI���ʂ̍X�V���\����
struct UpdateContext
{
	float elapsedTime;// �o�ߎ���
	float dashStamina;// �_�b�V���X�^�~�i
	float bulletPoint;// �e�Q�[�W
	float playerHP;// �v���C���[HP
};

// UI�̃C���^�[�t�F�[�X�N���X
class IUI
{
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IUI() = default;
	// ������
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	// �X�V
	virtual void Update(const UpdateContext& data) = 0;
	// �`��
	virtual void Render() = 0;
};
#endif		// IUI_DEFINED
