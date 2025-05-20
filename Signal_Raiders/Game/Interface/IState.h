#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
// �O���錾
class EnemyAI;
class IState
{
public:
	// �񋓌^
	enum  EnemyState//�G�̏��
	{
		IDLING = 0,//�p�j
		ATTACK,// �U��
		HIT,// �U����H�����
		ANGRY,// �{��
	};
public:
	virtual ~IState() = default;	// �f�X�g���N�^
	virtual void Initialize() = 0;	// ������
	virtual void Update(float elapsedTime) = 0;	// �X�V
};
#endif		// ISTATE_DEFINED
