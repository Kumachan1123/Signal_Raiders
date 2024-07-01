#pragma once
#include "pch.h"
#include <Game/CommonResources.h>
class IBullet
{
public:
	enum BulletState
	{
		UNUSED,
		FLYING,
		USED
	};
public:
	// ���z�f�X�g���N�^
	virtual ~IBullet() = default;
	// ����������
	virtual void Initialize(CommonResources* resources) = 0;
	// �X�V����
	virtual void Update(DirectX::SimpleMath::Vector3& Direction) = 0;
	// �`�悷��
	virtual void Render() = 0;
	// �㏈�����s��
	virtual void Finalize() = 0;
	// �C�e�����˂���Ă��邩�ǂ������擾����
	virtual BulletState GetBulletState() const = 0;
	// �C�e�̏�Ԃ�ݒ肷��
	virtual void SetBulletState(BulletState bulletState) = 0;

};