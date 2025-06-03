#pragma once
#ifndef IBULLET_DEFINED
#define IBULLET_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
// �O���錾
class CommonResources;
class IBullet
{
public:	// public�֐�
	// �f�X�g���N�^
	virtual ~IBullet() = default;
	// ������
	virtual void Initialize(CommonResources* resources) = 0;
	// �X�V
	virtual void Update(float elapsedTime) = 0;
	// �`��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// �e�̕`��
	virtual void RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
};
#endif		// IBULLET_DEFINED
