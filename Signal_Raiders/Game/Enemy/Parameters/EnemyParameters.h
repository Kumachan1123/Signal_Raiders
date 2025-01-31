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
	static const float ENEMY_SPAWN_RADIUS;	// �U�R�G���o�����锼�a
	static const float RANDOM_MAX;  // �����_���̍ő�l
	static const float RANDOM_MIN;  // �����_���̍ŏ��l
	static const float RANDOM_ROTATION_MAX;  // �����_���̍ő�l
	static const float RANDOM_ROTATION_MIN;  // �����_���̍ŏ��l
	static const float AMPLITUDE;  // �U��
	static const float FREQUENCY;  // ���g��
	static const float MOVE_SPEED;  // �ړ����x
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
	static const float NORMAL_ENEMY_RADIUS;	// �U�R�G�̋��E���̔��a
	static const float NORMAL_BOSS_RADIUS;	// �ʏ펞�̃{�X�̋��E���̔��a
	static const float NORMAL_ENEMY_DAMAGE;	// �U�R�G���v���C���[�ɗ^����_���[�W
	static const float ATTACK_INTERVAL;	// �G�̍U���C���^�[�o��
	static const float BOSS_DAMAGE;	// �{�X���v���C���[�ɗ^����_���[�W
	static const float BOSS_BULLET_ANGLE;	// �{�X�̒e�̊p�x
	static const float BOSS_HPBAR_SCALE;	// �{�X��HP�o�[�̃X�P�[��
	static const float BOSS_CHASE_SPEED;	// �{�X�̒ǐՑ��x
	static const float BOSS_DEADEFFECT_SCALE;	// �{�X�̎��S�G�t�F�N�g�̃X�P�[��
	static const float ENEMY_DEADEFFECT_SCALE;	// �U�R�G�̎��S�G�t�F�N�g�̃X�P�[��
	static const float ENEMY_SPAWN_START_TIME;	// �G���o�����n�߂鎞��
	static const float ENEMY_SPAWN_INTERVAL;// �G���o������Ԋu
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY; // ���V�̏������x
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_POSITION; // �{�X�̏����ʒu
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE; // �{�X�̏����X�P�[��
	static const DirectX::SimpleMath::Vector3 BOSS_SPHERE_OFFSET; // �{�X�̋��E���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_HEAD_OFFSET;	// �{�X�̓��̖C���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_LEFT_GUN_OFFSET;	// �{�X�̍��̖C���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_RIGHT_GUN_OFFSET;// �{�X�̉E�̖C���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_HPBAR_OFFSET;	// �{�X�̃V�[���h�̃I�t�Z�b�g
	static const KnockBackTime KNOCKBACK_TIME;// �m�b�N�o�b�N�J�n����o�߂�������
	static const DotClamp DOT_CLAMP;// ���ς�clamp����Ƃ��Ɏg���l

};