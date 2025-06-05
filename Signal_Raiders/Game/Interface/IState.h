/*
*	@file IState.h
*	@brief �G�̏�Ԃ��Ǘ�����C���^�[�t�F�[�X�N���X
*/
#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED
// �W�����C�u����
#include <SimpleMath.h>

// �O���錾
class EnemyAI;

// �G�̏�Ԃ��Ǘ�����C���^�[�t�F�[�X�N���X
class IState
{
public:
	// �񋓌^
	//�G�̏��
	enum  EnemyState
	{
		IDLING = 0,//�p�j
		ATTACK,// �U��
		HIT,// �U����H�����
		ANGRY,// �{��
		KNOCKBACK,// �m�b�N�o�b�N
		CHASE,// �ǐ�
		SHADOWSTEP,//��荞��
	};
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IState() = default;
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update(float elapsedTime) = 0;
};
#endif		// ISTATE_DEFINED
