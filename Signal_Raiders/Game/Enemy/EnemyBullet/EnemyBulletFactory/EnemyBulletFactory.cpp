/*
*	@file	EnemyBulletFactory.cpp
*	@brief	�G�e�t�@�N�g���[�N���X
*/
#include "pch.h"
#include "EnemyBulletFactory.h"
/*
*	@brief	�e�̐���
*	@details �G�e�t�@�N�g���[�N���X�̐ÓI�֐�
*	@param	BulletType type �e�̎��
*	@return	���������e�̃|�C���^
*/
std::unique_ptr<IEnemyBullet> EnemyBulletFactory::CreateBullet(BulletType type)
{
	// �e�̎�ނɉ����Ēe�𐶐�
	switch (type)
	{
	case BulletType::NORMAL:// �ʏ�e
		return std::make_unique<NormalBullet>();
	case BulletType::SPECIAL:// ����e
		return std::make_unique<SpecialBullet>();
	case BulletType::SPEED:// �����e
		return std::make_unique<SpeedBullet>();
	default:// �s���Ȓe�̎��
		return nullptr;
	}
};