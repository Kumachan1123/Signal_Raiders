#pragma once
#ifndef IBULLET_DEFINED
#define IBULLET_DEFINED
#include <SimpleMath.h>

class CommonResources;
class IBullet
{
public:// �֐�
	virtual ~IBullet() = default;
	virtual void Initialize(CommonResources* resources) = 0;// ������
	virtual void Update(float elapsedTime) = 0;// �X�V
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// �`��
	virtual void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// �e�̕`��
};
#endif		// IBULLET_DEFINED
