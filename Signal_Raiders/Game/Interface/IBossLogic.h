#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
// �O���錾
class Player;
class CommonResources;
class BulletManager;
class IBossLogic
{
public:
	virtual ~IBossLogic() = default;// �f�X�g���N�^
	virtual void Initialize() = 0;// ������
	virtual void ChangeState() = 0;// �X�V
	virtual void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// �`��
	virtual void BulletPositioning() = 0;// �e�̔��ˈʒu������
	virtual	void CreateBullet() = 0;// �e�𐶐�
};
#endif // IBOSSLOGIC_DEFINED