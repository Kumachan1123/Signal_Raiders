/*
	@file	EnemyParameters.h
	@brief	�GAI�p�p�����[�^�[�N���X
*/
#pragma once
class EnemyParameters
{
public:// �萔�p�\����
	// �m�b�N�o�b�N�J�n����o�߂�������
	struct KnockBackTime
	{
		float canAttackTime;    // �J�n������o���čU���ł���悤�ɂȂ鎞��
		float endKnockTime;     // �m�b�N�o�b�N��������߂鎞��
	};

	// ���ς�clamp����Ƃ��Ɏg���l
	struct DotClamp
	{
		float max;  // �ő�l
		float min;  // �ŏ��l
	};
public:// �萔

	// ===== ���ʁi�U�R�E�{�X�����Ɋ֌W������́j =====
	static const float RANDOM_MAX;// �����_���l�̍ő�l
	static const float RANDOM_MIN;// �����_���l�̍ŏ��l
	static const float RANDOM_ROTATION_MAX;// �����_����]�̍ő�l
	static const float RANDOM_ROTATION_MIN;// �����_����]�̍ŏ��l
	static const float AMPLITUDE;// �U��
	static const float FREQUENCY; // ���g��
	static const float MOVE_SPEED;// �ړ����x
	static const float FIXED_INITIAL_SPEED;// �Œ菉��
	static const float KNOCKBACK_DURATION;// �m�b�N�o�b�N�̎�������
	static const float KNOCKBACK_PROGRESS_MAX;// �m�b�N�o�b�N�̍ő�i�s�x
	static const float KNOCKBACK_DECAY_RATE;// �m�b�N�o�b�N�̌�����
	static const float ATTACK_COOLDOWN;// �U���N�[���_�E������
	static const float INITIAL_ROTATION_SPEED;// ������]���x
	static const float ROTATION_SPEED_DIVISOR;// ��]���x�̕���
	static const float ROTATION_SPEED_MIN;// ��]���x�̍ŏ��l
	static const float ANGLE_THRESHOLD;// �p�x��臒l
	static const float CHANGE_FACE_TIME;// �\���ς��鎞��
	static const float VELOCITY_SCALE_FACTOR;// �v���C���[�̕����Ɉړ�����ۂ̑��x�X�P�[���t�@�N�^�[
	static const float ATTACK_COOLDOWN_THRESHOLD;// �U���N�[���_�E����臒l
	static const float SPECIAL_ATTACK_COOLDOWN;// ����U���̃N�[���_�E������
	static const float ATTACK_INTERVAL;// �U���Ԋu
	static const float ENEMY_BULLET_SIZE;// �G�e�̃T�C�Y
	static const float BOSS_BULLET_SIZE;// �{�X�e�̃T�C�Y
	static const KnockBackTime KNOCKBACK_TIME;// �m�b�N�o�b�N����
	static const DotClamp DOT_CLAMP;// ���ς�clamp����Ƃ��Ɏg���l
	// ===== �U�R�G��p =====
	static const float ENEMY_SPAWN_RADIUS;// �G�̃X�|�[�����a
	static const float NORMAL_ENEMY_RADIUS;// �ʏ�G�̔��a
	static const float NORMAL_ENEMY_DAMAGE;// �ʏ�G�̃_���[�W
	static const float ENEMY_DEADEFFECT_SCALE;// �G�̃f�X�G�t�F�N�g�̃X�P�[��
	static const float ENEMY_SPAWN_START_TIME;// �G�̃X�|�[���J�n����
	static const float ENEMY_SPAWN_INTERVAL; // �G�̃X�|�[���Ԋu
	static const DirectX::SimpleMath::Vector3 ENEMY_HPBAR_OFFSET;// �U�RHP�o�[�̃I�t�Z�b�g
	// ===== �{�X���� =====
	static const float BOSS_SHIELD_SCALE_SPEED;// �{�X�V�[���h�̊g�呬�x
	static const float BOSS_SHIELD_RADIUS; // �{�X�V�[���h�̔��a
	static const float BOSS_SHIELD_ATTACK_COOLDOWN; // �{�X�V�[���h�̍U���N�[���_�E������
	static const float NORMAL_BOSS_RADIUS;// �ʏ�{�X�̔��a
	static const float BOSS_DAMAGE; // �{�X�̃_���[�W
	static const float BOSS_BULLET_ANGLE; // �{�X�e�̊p�x
	static const float BOSS_HPBAR_SCALE;// �{�XHP�o�[�̃X�P�[��
	static const float BOSS_CHASE_SPEED; // �{�X�̒ǐՑ��x
	static const float BOSS_DEADEFFECT_SCALE; // �{�X�̃f�X�G�t�F�N�g�̃X�P�[��
	static const float BOSS_SPAWN_WAIT_TIME; // �{�X�̃X�|�[���ҋ@����

	// ===== �{�X��p =====
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_POSITION;// �{�X�̏����ʒu
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE; // �{�X�̏����X�P�[��
	static const DirectX::SimpleMath::Vector3 BOSS_SPHERE_OFFSET;// �{�X�̋��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_HEAD_OFFSET;// �{�X�̓��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_LEFT_GUN_OFFSET;// �{�X�̍��K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_RIGHT_GUN_OFFSET;// �{�X�̉E�K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_HPBAR_OFFSET;// �{�XHP�o�[�̃I�t�Z�b�g

	// ===== ���X�{�X��p =====
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_POSITION;// �{�X�̏����ʒu
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_SCALE; // �{�X�̏����X�P�[��
	static const DirectX::SimpleMath::Vector3 LASTBOSS_SPHERE_OFFSET;// �{�X�̋��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HEAD_OFFSET;// �{�X�̓��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_LEFT_GUN_OFFSET;// �{�X�̍��K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_RIGHT_GUN_OFFSET;// �{�X�̉E�K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HPBAR_OFFSET;// �{�XHP�o�[�̃I�t�Z�b�g
	// ===== ���V�E����ȏ�����ԂȂ� =====
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY; // ���V�p�̏����x

};