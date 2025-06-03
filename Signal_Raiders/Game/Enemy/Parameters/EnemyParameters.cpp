/*
	@file	EnemyParameters.cpp
	@brief	�GAI�p�p�����[�^�[�N���X
*/
#include <pch.h>
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

// �萔
// ==============================
// ���ʁi�{�X�E�U�R�����j
// ==============================
// �����_���̍ő�l
const float EnemyParameters::RANDOM_MAX = 7.0f;
// �����_���̍ŏ��l
const float EnemyParameters::RANDOM_MIN = 5.5f;
// �����_����]�̍ő�l
const float EnemyParameters::RANDOM_ROTATION_MAX = 2.0f;
// �����_����]�̍ŏ��l
const float EnemyParameters::RANDOM_ROTATION_MIN = 0.5f;
// �U��
const float EnemyParameters::AMPLITUDE = 2.0f;
// ���g��
const float EnemyParameters::FREQUENCY = 0.5f;
// �ړ����x
const float EnemyParameters::MOVE_SPEED = 20.0f;
// �Œ菉��
const float EnemyParameters::FIXED_INITIAL_SPEED = 20.0f;
// �m�b�N�o�b�N�̎�������
const float EnemyParameters::KNOCKBACK_DURATION = 2.0f;
// �m�b�N�o�b�N�̍ő�i�s�x
const float EnemyParameters::KNOCKBACK_PROGRESS_MAX = 2.0f;
// �m�b�N�o�b�N�̌�����
const float EnemyParameters::KNOCKBACK_DECAY_RATE = -3.0f;
// �U���N�[���_�E������
const float EnemyParameters::ATTACK_COOLDOWN = 3.0f;
// ������]���x
const float EnemyParameters::INITIAL_ROTATION_SPEED = 1.0f;
// ��]���x�̕���
const float EnemyParameters::ROTATION_SPEED_DIVISOR = 10.0f;
// ��]���x�̍ŏ��l
const float EnemyParameters::ROTATION_SPEED_MIN = 0.24f;
// �p�x��臒l
const float EnemyParameters::ANGLE_THRESHOLD = 0.1f;
// �\���ς��鎞��
const float EnemyParameters::CHANGE_FACE_TIME = 2.0f;
// �v���C���[�̕����Ɉړ�����ۂ̑��x�X�P�[���t�@�N�^�[
const float EnemyParameters::VELOCITY_SCALE_FACTOR = 5.0f;
// �U���N�[���_�E����臒l
const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;
// ����U���N�[���_�E������
const float EnemyParameters::SPECIAL_ATTACK_COOLDOWN = 30.0f;
// �U���Ԋu
const float EnemyParameters::ATTACK_INTERVAL = 1.0f;
// �e�̃T�C�Y
const float EnemyParameters::ENEMY_BULLET_SIZE = 0.15f;
// �{�X�e�̃T�C�Y
const float EnemyParameters::BOSS_BULLET_SIZE = 0.25f;
// ���X�{�X�e�̃T�C�Y
const float EnemyParameters::LASTBOSS_BULLET_SIZE = 0.75f;
// �m�b�N�o�b�N����
const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };
// �m�b�N�o�b�N���Ԃ̃N�����v
const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };


// ==============================
// �U�R�G��p
// ==============================
// �G�̃X�|�[�����a
const float EnemyParameters::ENEMY_SPAWN_RADIUS = 50.0f;
// �ʏ�G�̔��a
const float EnemyParameters::NORMAL_ENEMY_RADIUS = 1.5f;
// �ʏ�G�̃_���[�W
const float EnemyParameters::NORMAL_ENEMY_DAMAGE = 1.0f;
// �ʏ�G��HP
const float EnemyParameters::ENEMY_DEADEFFECT_SCALE = 3.0f;
// �G�̃X�|�[���J�n����
const float EnemyParameters::ENEMY_SPAWN_START_TIME = 5.0f;
// �G�̃X�|�[���Ԋu
const float EnemyParameters::ENEMY_SPAWN_INTERVAL = 0.5f;
// �U�RHP�o�[�̃I�t�Z�b�g
const Vector3 EnemyParameters::ENEMY_HPBAR_OFFSET{ 0.0f, 1.0f, 0.0f };


// ==============================
// �{�X����
// ==============================
// �{�X�V�[���h�̊g�呬�x
const float EnemyParameters::BOSS_SHIELD_SCALE_SPEED = 0.2f;
// �{�X�V�[���h�̍U���N�[���_�E������
const float EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN = 0.75f;
// �{�XHP�o�[�̃X�P�[��
const float EnemyParameters::BOSS_HPBAR_SCALE = 3.0f;
// �{�X�̒ǐՑ��x
const float EnemyParameters::BOSS_CHASE_SPEED = 2.0f;
// �{�X�̃X�|�[���ҋ@����
const float EnemyParameters::BOSS_SPAWN_WAIT_TIME = 3.0f;

// ===============================
// �{�X��p
// ===============================
// �{�X�̏����ʒu
const Vector3 EnemyParameters::INITIAL_BOSS_POSITION{ 0.0f, 5.0f, 0.0f };
// �{�X�̏����X�P�[��
const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f, 2.0f, 2.0f };
// �{�X�̋��̃I�t�Z�b�g
const Vector3 EnemyParameters::BOSS_SPHERE_OFFSET{ 0.0f, 0.5f, 0.0f };
// �{�X�̓��̃I�t�Z�b�g
const Vector3 EnemyParameters::BOSS_HEAD_OFFSET{ 0.0f,2.0f, 3.0f };
// �{�X�̍��K���̃I�t�Z�b�g
const Vector3 EnemyParameters::BOSS_LEFT_GUN_OFFSET{ -2.5f, -0.50f, 3.0f };
// �{�X�̉E�K���̃I�t�Z�b�g
const Vector3 EnemyParameters::BOSS_RIGHT_GUN_OFFSET{ 2.5f, -0.50f, 3.0f };
// �{�XHP�o�[�̃I�t�Z�b�g
const Vector3 EnemyParameters::BOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };
// �ʏ�{�X�̔��a
const float EnemyParameters::NORMAL_BOSS_RADIUS = 3.0f;
// �{�X�V�[���h�̔��a
const float EnemyParameters::BOSS_SHIELD_RADIUS = 5.5f;
// �{�X�V�[���h�̃T�C�Y
const float EnemyParameters::BOSS_SHIELD_SIZE = 3.0f;
// �{�X���v���C���[�ɗ^����_���[�W
const float EnemyParameters::BOSS_DAMAGE = 5.0f;
// �o���A�j��p�[�e�B�N���̃T�C�Y
const float EnemyParameters::BOSS_BARRIERBREAK_SIZE = 1.0f;
// �{�X�̃f�X�G�t�F�N�g�̃X�P�[��
const float EnemyParameters::BOSS_DEADEFFECT_SCALE = 10.0f;

// ================================
// ���X�{�X��p
// ================================
// �{�X�̏����ʒu
const Vector3 EnemyParameters::INITIAL_LASTBOSS_POSITION{ 0.0f, 10.0f, 0.0f };
// �{�X�̏����X�P�[��
const Vector3 EnemyParameters::INITIAL_LASTBOSS_SCALE{ 1.0f, 1.0f, 1.0f };
// ���X�{�X�̋��̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_SPHERE_OFFSET{ 0.0f, 0.5f, 0.0f };
// ���X�{�X�̓��̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_HEAD_OFFSET{ 0.0f, -1.0f, 3.0f };
// ���X�{�X�̍��K���̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET{ -9.5f, 4.50f, 9.0f };
// ���X�{�X�̉E�K���̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET{ 9.5f, 4.50f, 9.0f };
// ���X�{�X�̍����K���̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET{ -9.5f, -3.50f, 9.0f };
// ���X�{�X�̉E���K���̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET{ 9.5f, -3.50f, 9.0f };
// ���X�{�XHP�o�[�̃I�t�Z�b�g
const Vector3 EnemyParameters::LASTBOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };
// �ʏ�{�X�̔��a
const float EnemyParameters::NORMAL_LASTBOSS_RADIUS = 8.5f;
// �{�X�V�[���h�̔��a
const float EnemyParameters::LASTBOSS_SHIELD_RADIUS = 20.5f;
// �{�X�V�[���h�̃T�C�Y
const float EnemyParameters::LASTBOSS_SHIELD_SIZE = 10.0f;
// ���X�{�X���v���C���[�ɗ^����_���[�W
const float EnemyParameters::LASTBOSS_DAMAGE = 10.0f;
// �o���A�j��p�[�e�B�N���̃T�C�Y
const float EnemyParameters::LASTBOSS_BARRIERBREAK_SIZE = 2.0f;
// ���X�{�X�̃f�X�G�t�F�N�g�̃X�P�[��
const float EnemyParameters::LASTBOSS_DEADEFFECT_SCALE = 15.0f;



// ==============================
// ���o�E����p�r
// ==============================
// �G�̏����ʒu
const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f, 0.5f, 0.0f };
