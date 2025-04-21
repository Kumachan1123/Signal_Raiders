#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
class Player;
class CommonResources;
class BulletManager;

class IBossLogic
{
public:
	virtual ~IBossLogic() = default;                                          // �f�X�g���N�^
	virtual void CreateModel() = 0;                                             // ������
	virtual void ChangeState() = 0;                                // �X�V
	virtual void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;        // �`��
};
#endif // IBOSSLOGIC_DEFINED
