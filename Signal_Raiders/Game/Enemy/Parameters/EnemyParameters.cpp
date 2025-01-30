/*
	@file	EnemyParameters.cpp
	@brief	�GAI�p�p�����[�^�[�N���X
*/
#include "pch.h"
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

const float EnemyParameters::RANDOM_MAX = 7.0f;// �����_���̍ő�l

const float EnemyParameters::RANDOM_MIN = 5.5f;// �����_���̍ŏ��l

const float EnemyParameters::AMPLITUDE = 2.0f;// �U��

const float EnemyParameters::FREQUENCY = 0.5f;// ���g��

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

const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;// �t���[�����Ƃɒe�𔭎˂��Ȃ����邽�߂̍U��臒l

const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f,0.5f,0.0f };// ���V�̏������x

const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f,2.0f,2.0f };// �{�X�̏����X�P�[��

const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };// �m�b�N�o�b�N�J�n����o�߂�������

const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };// ���ς�clamp����Ƃ��Ɏg���l


