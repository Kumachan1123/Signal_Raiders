/*
	@file	Boss.h
	@brief	�{�X�N���X
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/Boss/BossModel/BossModel.h"


//�O���錾
class CommonResources;
class BossBase;
class BossAI;
class BossModel;
class Boss : public IBossLogic
{
public:
	// public�֐�
	Boss(BossBase* pBoss, CommonResources* commonResources);// �R���X�g���N�^
	~Boss();// �f�X�g���N�^
	void Initialize()override; // ������(Boss, LastBoss�ɂă��f���𐶐�)
	void ChangeState()override;// ��ԕύX(Boss,LastBoss�ɂď�Ԃ�ύX)
	void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// �`��(Boss, LastBoss�ɂĕ`��)
	void BulletPositioning()override;	// �e�̔��ˈʒu������
	void CreateBullet();// �e�𐶐�
	void CreateCenterBullet(BulletType type);// �����̒e�𐶐�
	void CreateLeftBullet(BulletType type);// ���̒e�𐶐�
	void CreateRightBullet(BulletType type);// �E�̒e�𐶐�
private:
	// private�����o�ϐ�
	CommonResources* m_pCommonResources;	// ���ʃ��\�[�X
	std::unique_ptr<BossModel>		m_pBossModel;//���f��
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HP�o�[
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::BoundingSphere m_bossBS;	//�G�̋��E��
	DirectX::SimpleMath::Matrix m_matrix;// �}�g���N�X
	// �e�̔��ˈʒu
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;	// ����
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;	// ��
	DirectX::SimpleMath::Vector3 m_bulletPosRight;	// �E
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// �e�̃^�C�v
	BulletType m_bulletType;// EnemyBullet�N���X�ɑ���
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �v���C���[�̃J�����̏��
	DirectX::SimpleMath::Vector3 m_cameraEye;// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraTarget;// �J�����̒����_
	DirectX::SimpleMath::Vector3 m_cameraUp;	// �J�����̏����
	// �{�X�x�[�X�N���X�̃|�C���^
	BossBase* m_pBossBase;
};