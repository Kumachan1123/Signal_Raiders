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
	// �����_���l�̍ő�l
	static const float RANDOM_MAX;
	// �����_���l�̍ŏ��l
	static const float RANDOM_MIN;
	// �����_����]�̍ő�l
	static const float RANDOM_ROTATION_MAX;
	// �����_����]�̍ŏ��l
	static const float RANDOM_ROTATION_MIN;
	// �U��
	static const float AMPLITUDE;
	// ���g��
	static const float FREQUENCY;
	// �ړ����x
	static const float MOVE_SPEED;
	// �Œ菉��
	static const float FIXED_INITIAL_SPEED;
	// �m�b�N�o�b�N�̎�������
	static const float KNOCKBACK_DURATION;
	// �m�b�N�o�b�N�̍ő�i�s�x
	static const float KNOCKBACK_PROGRESS_MAX;
	// �m�b�N�o�b�N�̌�����
	static const float KNOCKBACK_DECAY_RATE;
	// �U���N�[���_�E������
	static const float ATTACK_COOLDOWN;
	// ������]���x
	static const float INITIAL_ROTATION_SPEED;
	// ��]���x�̕���
	static const float ROTATION_SPEED_DIVISOR;
	// ��]���x�̍ŏ��l
	static const float ROTATION_SPEED_MIN;
	// �p�x��臒l
	static const float ANGLE_THRESHOLD;
	// �\���ς��鎞��
	static const float CHANGE_FACE_TIME;
	// �v���C���[�̕����Ɉړ�����ۂ̑��x�X�P�[���t�@�N�^�[
	static const float VELOCITY_SCALE_FACTOR;
	// �U���N�[���_�E����臒l
	static const float ATTACK_COOLDOWN_THRESHOLD;
	// ����U���̃N�[���_�E������
	static const float SPECIAL_ATTACK_COOLDOWN;
	// �U���Ԋu
	static const float ATTACK_INTERVAL;
	// �G�e�̃T�C�Y
	static const float ENEMY_BULLET_SIZE;
	// �{�X�e�̃T�C�Y
	static const float BOSS_BULLET_SIZE;
	// ���X�{�X�e�̃T�C�Y
	static const KnockBackTime KNOCKBACK_TIME;
	// �m�b�N�o�b�N����
	static const float LASTBOSS_BULLET_SIZE;
	// ���ς�clamp����Ƃ��Ɏg���l
	static const DotClamp DOT_CLAMP;
	// ===== �U�R�G��p =====
	// �G�̃X�|�[�����a
	static const float ENEMY_SPAWN_RADIUS;
	// �ʏ�G�̔��a
	static const float NORMAL_ENEMY_RADIUS;
	// �ʏ�G�̃_���[�W
	static const float NORMAL_ENEMY_DAMAGE;
	// �G�̃f�X�G�t�F�N�g�̃X�P�[��
	static const float ENEMY_DEADEFFECT_SCALE;
	// �G�̃X�|�[���J�n����
	static const float ENEMY_SPAWN_START_TIME;
	// �G�̃X�|�[���Ԋu
	static const float ENEMY_SPAWN_INTERVAL;
	// �U�RHP�o�[�̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 ENEMY_HPBAR_OFFSET;
	// ===== �{�X���� =====
	// �{�X�V�[���h�̊g�呬�x
	static const float BOSS_SHIELD_SCALE_SPEED;
	// �{�X�V�[���h�̍U���N�[���_�E������
	static const float BOSS_SHIELD_ATTACK_COOLDOWN;
	// �{�XHP�o�[�̃X�P�[��
	static const float BOSS_HPBAR_SCALE;
	// �{�X�̒ǐՑ��x
	static const float BOSS_CHASE_SPEED;
	// �{�X�̃X�|�[���ҋ@����
	static const float BOSS_SPAWN_WAIT_TIME;

	// ===== �{�X��p =====
	// �{�X�̏����ʒu
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_POSITION;
	// �{�X�̏����X�P�[��
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE;
	// �{�X�̋��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_SPHERE_OFFSET;
	// �{�X�̓��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_HEAD_OFFSET;
	// �{�X�̍��K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_LEFT_GUN_OFFSET;
	// �{�X�̉E�K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_RIGHT_GUN_OFFSET;
	// �{�XHP�o�[�̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 BOSS_HPBAR_OFFSET;
	// �ʏ�{�X�̔��a
	static const float NORMAL_BOSS_RADIUS;
	// �{�X�V�[���h�̔��a
	static const float BOSS_SHIELD_RADIUS;
	// �{�X�V�[���h�̃T�C�Y
	static const float BOSS_SHIELD_SIZE;
	// �{�X���v���C���[�ɗ^����_���[�W
	static const float BOSS_DAMAGE;
	// �o���A�j��p�[�e�B�N���̃T�C�Y
	static const float BOSS_BARRIERBREAK_SIZE;
	// �{�X�̃f�X�G�t�F�N�g�̃X�P�[��
	static const float BOSS_DEADEFFECT_SCALE;


	// ===== ���X�{�X��p =====
	// �{�X�̏����ʒu
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_POSITION;
	// �{�X�̏����X�P�[��
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_SCALE;
	// �{�X�̋��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_SPHERE_OFFSET;
	// �{�X�̓��̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HEAD_OFFSET;
	// �{�X�̍���K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_LEFTUP_GUN_OFFSET;
	// �{�X�̉E��K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_RIGHTUP_GUN_OFFSET;
	// �{�X�̍����K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_LEFTDOWN_GUN_OFFSET;
	// �{�X�̉E���K���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_RIGHTDOWN_GUN_OFFSET;
	// �{�XHP�o�[�̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HPBAR_OFFSET;
	// �ʏ�{�X�̔��a
	static const float NORMAL_LASTBOSS_RADIUS;
	// �{�X�V�[���h�̔��a
	static const float LASTBOSS_SHIELD_RADIUS;
	// �{�X�V�[���h�̃T�C�Y
	static const float LASTBOSS_SHIELD_SIZE;
	// ���X�{�X���v���C���[�ɗ^����_���[�W
	static const float LASTBOSS_DAMAGE;
	// �o���A�j��p�[�e�B�N���̃T�C�Y
	static const float LASTBOSS_BARRIERBREAK_SIZE;
	// ���X�{�X�̃f�X�G�t�F�N�g�̃X�P�[��
	static const float LASTBOSS_DEADEFFECT_SCALE;

	// ===== ���V�E����ȏ�����ԂȂ� =====
	// ���V�p�̏����x
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY;

};