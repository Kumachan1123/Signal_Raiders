/*
	@file	Boss.h
	@brief	�{�X�N���X
*/
#pragma once
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
class BossModel;
class BossAI;
class Boss : public IBossLogic
{
public:	// public�֐�
	// �R���X�g���N�^
	Boss(BossBase* pBoss, CommonResources* commonResources);
	// �f�X�g���N�^
	~Boss();
	// ������
	void Initialize()override;
	// ��ԕύX
	void ChangeState()override;
	// �`��
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// �e�̔��ˈʒu������
	void BulletPositioning()override;
	// �e�𐶐�
	void CreateBullet();
	// �����̒e�𐶐�
	void CreateCenterBullet(BulletType type);
	// ���̒e�𐶐�
	void CreateLeftBullet(BulletType type);
	// �E�̒e�𐶐�
	void CreateRightBullet(BulletType type);
private:// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	//���f��
	std::unique_ptr<BossModel>		m_pBossModel;
	// HP�o�[
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]
	DirectX::SimpleMath::Vector3 m_rotate;
	// �G�̋��E��
	DirectX::BoundingSphere m_bossBS;
	// �}�g���N�X
	DirectX::SimpleMath::Matrix m_matrix;
	// �e�̔��ˈʒu: ����
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// �e�̔��ˈʒu: ��
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;
	// �e�̔��ˈʒu: �E
	DirectX::SimpleMath::Vector3 m_bulletPosRight;
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	//  EnemyBullet�N���X�ɑ���e�̃^�C�v
	BulletType m_bulletType;
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// �J�����̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �J�����̏����
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �{�X�x�[�X�N���X�̃|�C���^
	BossBase* m_pBossBase;
};