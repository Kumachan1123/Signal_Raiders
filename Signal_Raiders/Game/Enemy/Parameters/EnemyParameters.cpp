/*
	@file	EnemyParameters.cpp
	@brief	�GAI�p�p�����[�^�[�N���X
*/
#include "pch.h"
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

// �萔

const float EnemyParameters::ENEMY_SPAWN_RADIUS = 50.0f;// �U�R�G���o�����锼�a

const float EnemyParameters::RANDOM_MAX = 7.0f;// �����_���̍ő�l

const float EnemyParameters::RANDOM_MIN = 5.5f;// �����_���̍ŏ��l

const float EnemyParameters::RANDOM_ROTATION_MAX = 2.0f;// �����_���̍ő�l

const float EnemyParameters::RANDOM_ROTATION_MIN = 0.5f;// �����_���̍ŏ��l

const float EnemyParameters::AMPLITUDE = 2.0f;// �U��

const float EnemyParameters::FREQUENCY = 0.5f;// ���g��

const float EnemyParameters::MOVE_SPEED = 20.0f;// �ړ����x

const float EnemyParameters::FIXED_INITIAL_SPEED = 20.0f;// �m�b�N�o�b�N�̏����x

const float EnemyParameters::KNOCKBACK_DURATION = 2.0f;// �m�b�N�o�b�N�̒����i�b�j

const float EnemyParameters::KNOCKBACK_PROGRESS_MAX = 2.0f;// �m�b�N�o�b�N�i�s�x�̍ő�l

const float EnemyParameters::KNOCKBACK_DECAY_RATE = -3.0f;// �m�b�N�o�b�N�̌�����

const float EnemyParameters::ATTACK_COOLDOWN = 3.0f;// �U���̃N�[���_�E���^�C��

const float EnemyParameters::INITIAL_ROTATION_SPEED = 1.0f;// ��]���x

const float EnemyParameters::ROTATION_SPEED_DIVISOR = 10.0f;// ��]���x�ɏ��Z����Ƃ��Ɏg���l

const float EnemyParameters::ROTATION_SPEED_MIN = 0.24f;// ��]���x�̍ŏ��l

const float EnemyParameters::ANGLE_THRESHOLD = 0.1f;// �p�x�̂������l

const float EnemyParameters::CHANGE_FACE_TIME = 2.0f;// �\���ς��鎞��

const float EnemyParameters::VELOCITY_SCALE_FACTOR = 5.0f;// ���K���������x�ɏ�Z����l

const float EnemyParameters::BOSS_SHIELD_SCALE_SPEED = 0.2f;// �{�X�̃V�[���h���傫���Ȃ鑬��

const float EnemyParameters::BOSS_SHIELD_RADIUS = 5.5f;// �V�[���h�W�J���̃{�X�̋��E���̔��a

const float EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN = 0.75f;// �V�[���h�W�J���̃{�X�̍U���Ԋu

const float EnemyParameters::NORMAL_BOSS_RADIUS = 2.5f;// �ʏ펞�̃{�X�̋��E���̔��a

const float EnemyParameters::NORMAL_ENEMY_RADIUS = 1.5f;// �U�R�G�̋��E���̔��a

const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;// �t���[�����Ƃɒe�𔭎˂��Ȃ����邽�߂̍U��臒l

const float EnemyParameters::NORMAL_ENEMY_DAMAGE = 1.0f;// �U�R�G���v���C���[�ɗ^����_���[�W

const float EnemyParameters::ATTACK_INTERVAL = 1.0f;// �U�R�G�̍U���C���^�[�o��

const float EnemyParameters::BOSS_DAMAGE = 5.0f;// �{�X���v���C���[�ɗ^����_���[�W

const float EnemyParameters::BOSS_BULLET_ANGLE = 30.0f;// �{�X�̒e�̊p�x

const float EnemyParameters::BOSS_HPBAR_SCALE = 3.0f;// �{�X��HP�o�[�̃X�P�[��

const float EnemyParameters::BOSS_CHASE_SPEED = 2.0f;// �{�X�̒ǐՑ��x

const float EnemyParameters::BOSS_DEADEFFECT_SCALE = 10.0f;// �{�X�̎��S�G�t�F�N�g�̃X�P�[��

const float EnemyParameters::ENEMY_DEADEFFECT_SCALE = 3.0f;// �U�R�G�̎��S�G�t�F�N�g�̃X�P�[��

const float EnemyParameters::ENEMY_SPAWN_START_TIME = 5.0f;// �G���o�����n�߂鎞��

const float EnemyParameters::ENEMY_SPAWN_INTERVAL = 0.5f;// �G���o������Ԋu

const float EnemyParameters::ENEMY_BULLET_SIZE = 0.15f;// �G�̒e�̑傫��

const float EnemyParameters::BOSS_BULLET_SIZE = 0.25f;// �{�X�̒e�̑傫��

const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f,0.5f,0.0f };// ���V�̏������x

const Vector3 EnemyParameters::INITIAL_BOSS_POSITION{ 0.0f,10.0f,0.0f };// �{�X�̏����ʒu

const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f,2.0f,2.0f };// �{�X�̏����X�P�[��

const Vector3 EnemyParameters::BOSS_SPHERE_OFFSET{ 0.0f,0.5f,0.0f };// �{�X�̋��E���̃I�t�Z�b�g

const Vector3 EnemyParameters::BOSS_HEAD_OFFSET{ 0.0f, 2.5f, 3.0f };// �{�X�̓��̖C���̃I�t�Z�b�g

const Vector3 EnemyParameters::BOSS_LEFT_GUN_OFFSET{ -2.5f, 1.0f, 3.0f };// ��̍��W

const Vector3 EnemyParameters::BOSS_RIGHT_GUN_OFFSET{ 2.5f, 1.0f, 3.0f };// �E�̍��W

const Vector3 EnemyParameters::BOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };// HP�o�[�̃I�t�Z�b�g

const Vector3 EnemyParameters::ENEMY_HPBAR_OFFSET{ 0.0f, 1.0f, 0.0f };// HP�o�[�̃I�t�Z�b�g

const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };// �m�b�N�o�b�N�J�n����o�߂�������

const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };// ���ς�clamp����Ƃ��Ɏg���l


