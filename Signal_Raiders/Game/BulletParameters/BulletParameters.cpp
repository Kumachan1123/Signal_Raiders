/*
	@file	BulletParameters.cpp
	@brief	�e�p�����[�^�[�N���X
*/
#include "pch.h"
#include "BulletParameters.h"

using namespace DirectX::SimpleMath;

//======================================
// �v���C���[�p�̒e�̒萔
//======================================
const float BulletParameters::PLAYER_BULLET_SIZE = 0.1f;// �e�̑傫��

const float BulletParameters::COLLISION_RADIUS = 0.25f;// �e�̓����蔻��̔��a

const float BulletParameters::ADJUST_MOVE = 40.0f;// �ړ��ʂ̕␳�l

const float BulletParameters::ADJUST_DIRECTION = 0.0375f;	// Direction���኱������ɂ��邽�߂̕␳�l

const Vector3  BulletParameters::INITIAL_POSITION = { 0.0f, -1.0f, 0.0f };	// ���ˈʒu

const float  BulletParameters::PLAYER_BULLET_LIFETIME = 1.5;				// ����

const int  BulletParameters::DAMAGE = 10;						// �G�ɗ^����_���[�W

const float  BulletParameters::RELOAD_INTERVAL = 0.1f;				// �����[�h�Ԋu
//======================================
// �G�p�̒e�̒萔
//======================================
const float  BulletParameters::ENEMY_BULLET_LIFETIME = 5.0f;			// ����

const float  BulletParameters::SPIRAL_BULLET_LIFETIME = 15.0f;			// ����

const float  BulletParameters::STRAIGHT_BULLET_SPEED = 10.0f;			// �����e�̑��x

const float  BulletParameters::STRAIGHT_ADJUST_DIRECTION = 0.05f;	// Direction���኱���ɂ��邽�߂̕␳�l

const float  BulletParameters::VERTICAL_BULLET_SPEED = 50.0f;			// �����e�̑��x

const Vector3  BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY = { 0.0f, -0.15f, 0.0f };	// ���e�O�̑��x

const float  BulletParameters::VERTICAL_BULLET_LANDING_POSITION = 0.5f;	// �����e�̒��e�ʒu

const float  BulletParameters::SPECIAL_ATTACK_WAIT_TIME = 5.0f;	// ����U���̑ҋ@����


//======================================
// ���ʂ̒萔�i�v���C���[�E�G���ʂŎg�����́j
//======================================
const float  BulletParameters::BULLET_ROTATION_SPEED = 6.0f;// �e�̎��]���x(�x)

const Plane  BulletParameters::SHADOW_PLANE = DirectX::SimpleMath::Plane(0.0f, 1.0f, 0.0f, 0.01f);// �e�̕���

const float  BulletParameters::ADJUST_HIT_VOLUME = 0.8f;// �q�b�gSE�{�����[������

const float  BulletParameters::ANGLE_MIN = 0.0f;// �p�x�̍ŏ��l

const float  BulletParameters::ANGLE_MAX = 360.0f;// �p�x�̍ő�l

const float  BulletParameters::DELETE_BULLET_POSITION = -0.25f;// �e�̍폜�ʒu

const int  BulletParameters::MAX_PLAYER_BULLET_COUNT = 50;// �v���C���[�̒e�̍ő吔

const float  BulletParameters::HIT_VOLUME = 0.8f;// �q�b�g���̃{�����[��

