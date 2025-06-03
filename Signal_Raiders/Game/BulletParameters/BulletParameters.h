/*
*	@file	BulletParameters.h
*	@brief	�e�p�����[�^�[�N���X
*/
#pragma once
class BulletParameters
{
public:
	//======================================
	// �v���C���[�p�̒e�̒萔
	//======================================
	static const float PLAYER_BULLET_SIZE;// �e�̑傫��
	static const float COLLISION_RADIUS;// �e�̓����蔻��̔��a
	static const float ADJUST_MOVE;// �ړ��ʂ̕␳�l
	static const float ADJUST_DIRECTION;	// Direction���኱������ɂ��邽�߂̕␳�l
	static const DirectX::SimpleMath::Vector3 INITIAL_POSITION;	// ���ˈʒu
	static const float PLAYER_BULLET_LIFETIME;				// ����
	static const int DAMAGE;						// �G�ɗ^����_���[�W
	static const float RELOAD_INTERVAL;				// �����[�h�Ԋu

	//======================================
	// �G�p�̒e�̒萔
	//======================================
	static const float ENEMY_BULLET_LIFETIME;			// ����(�����e�E�����e)
	static const float SPIRAL_BULLET_LIFETIME;			// ����(�����e)
	static const float STRAIGHT_BULLET_SPEED;			// �����e�̑��x
	static const float STRAIGHT_ADJUST_DIRECTION;	// Direction���኱���ɂ��邽�߂̕␳�l
	static const float VERTICAL_BULLET_SPEED;			// �����e�̑��x
	static const DirectX::SimpleMath::Vector3 VERTICAL_BULLET_LANDING_VELOCITY;	// ���e�O�̑��x
	static const float VERTICAL_BULLET_LANDING_POSITION;	// �����e�̒��e�ʒu
	static const float SPECIAL_ATTACK_WAIT_TIME;	// ����U���̑ҋ@����
	//======================================
	// ���ʂ̒萔�i�v���C���[�E�G���ʂŎg�����́j
	//======================================
	static const float DELETE_BULLET_POSITION;// �e�̍폜�ʒu
	static const float BULLET_ROTATION_SPEED;// �e�̎��]���x(�x)
	static const DirectX::SimpleMath::Plane SHADOW_PLANE;// �e�̕���
	static const float ADJUST_HIT_VOLUME;// �q�b�gSE�{�����[������
	static const float ANGLE_MIN;// �p�x�̍ŏ��l
	static const float ANGLE_MAX;// �p�x�̍ő�l
	static const float DELETE_BULLET_POSITION;// �e�̍폜�ʒu
	static const int MAX_PLAYER_BULLET_COUNT;// �v���C���[�̒e�̍ő吔
	static const float HIT_VOLUME;// �q�b�g���̃{�����[��
};