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

class IUI
{
public:
	virtual ~IUI() = default;// �f�X�g���N�^
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;// ������
	virtual void Update(const UpdateContext& data) = 0;// �X�V
	virtual void Render() = 0;// �`��
};
#endif		// IUI_DEFINED
