#pragma once
#include "pch.h"
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
	virtual void Initialize() = 0;
	// �X�V����
	virtual void Update() = 0;
	// �`�悷��
	virtual void Render() = 0;
	// �㏈�����s��
	virtual void Finalize() = 0;
};