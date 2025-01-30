/*
	@file	EnemyParameters.h
	@brief	�GAI�p�p�����[�^�[�N���X
*/
#pragma once
class EnemyParameters
{
public:// �萔�p�\����
	struct KnockBackTime// �m�b�N�o�b�N�J�n����o�߂�������
	{
		// �J�n������o���čU���ł���悤�ɂȂ鎞��
		float canAttackTime;
		// �m�b�N�o�b�N��������߂鎞��
		float endKnockTime;
	};
	struct DotClamp// ���ς�clamp����Ƃ��Ɏg���l
	{
		// �ő�l
		float max;
		// �ŏ��l
		float min;
	};
public:// �萔
	static const float RANDOM_MAX;  // �����_���̍ő�l
	static const float RANDOM_MIN;  // �����_���̍ŏ��l
	static const float AMPLITUDE;  // �U��
	static const float FREQUENCY;  // ���g��
	static const float FIXED_INITIAL_SPEED;// �m�b�N�o�b�N�̏����x
	static const float KNOCKBACK_DURATION;	// �m�b�N�o�b�N�̒����i�b�j
	static const float KNOCKBACK_PROGRESS_MAX; // �m�b�N�o�b�N�i�s�x�̍ő�l
	static const float KNOCKBACK_DECAY_RATE;  // �m�b�N�o�b�N�̌�����
	static const float ATTACK_COOLDOWN;  // �U���̃N�[���_�E���^�C��
	static const float INITIAL_ROTATION_SPEED; // ��]���x
	static const float ROTATION_SPEED_DIVISOR; // ��]���x�ɏ��Z����Ƃ��Ɏg���l
	static const float ROTATION_SPEED_MIN; // ��]���x�̍ŏ��l
	static const float ANGLE_THRESHOLD; // �p�x�̂������l
	static const float CHANGE_FACE_TIME;	// �\���ς��鎞��
	static const float VELOCITY_SCALE_FACTOR;	// ���K���������x�ɏ�Z����l
	static const float BOSS_SHIELD_SCALE_SPEED;	// �{�X�̃V�[���h���傫���Ȃ鑬��
	static const float BOSS_SHIELD_RADIUS;	// �V�[���h�W�J���̃{�X�̋��E���̔��a
	static const float BOSS_SHIELD_ATTACK_COOLDOWN;	// �V�[���h�W�J���̃{�X�̍U���Ԋu
	static const float ATTACK_COOLDOWN_THRESHOLD;	// �t���[�����Ƃɒe�𔭎˂��Ȃ����邽�߂̍U��臒l
	static const float NORMAL_BOSS_RADIUS;	// �ʏ펞�̃{�X�̋��E���̔��a
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY; // ���V�̏������x
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE; // �{�X�̏����X�P�[��
	static const KnockBackTime KNOCKBACK_TIME;// �m�b�N�o�b�N�J�n����o�߂�������
	static const DotClamp DOT_CLAMP;// ���ς�clamp����Ƃ��Ɏg���l

};