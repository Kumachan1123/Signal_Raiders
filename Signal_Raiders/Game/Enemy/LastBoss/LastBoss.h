/*
*  @file LastBoss.h
*  @brief ���X�{�X�N���X
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/LastBoss/LastBossModel/LastBossModel.h"

//�O���錾
class CommonResources;
class BossAI;
class LastBossModel;
class BossBase;
class LastBoss : public IBossLogic
{
public:// public�֐�

	// �R���X�g���N�^
	LastBoss(BossBase* pBoss, CommonResources* commonResources);
	// �f�X�g���N�^
	~LastBoss()override;
	// ������
	void Initialize()override;
	// ��Ԃ�ύX
	void ChangeState()override;
	// �`��
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// �e�̔��ˈʒu������
	void BulletPositioning()override;
	// �e�𐶐�
	void CreateBullet()override;
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	//���f��
	std::unique_ptr<LastBossModel>		m_pBossModel;
	// HP�o�[
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// �G�̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �G�̑��x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �G�̉�]
	DirectX::SimpleMath::Vector3 m_rotate;
	//�G�̋��E��
	DirectX::BoundingSphere m_bossBS;
	// �}�g���N�X
	DirectX::SimpleMath::Matrix m_matrix;
	// �e�̔��ˈʒu:�E��
	DirectX::SimpleMath::Vector3 m_bulletPosRightUp;
	// �e�̔��ˈʒu:�E��
	DirectX::SimpleMath::Vector3 m_bulletPosRightDown;
	// �e�̔��ˈʒu:����
	DirectX::SimpleMath::Vector3 m_bulletPosLeftUp;
	// �e�̔��ˈʒu:����
	DirectX::SimpleMath::Vector3 m_bulletPosLeftDown;
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// EnemyBullet�N���X�ɑ���e�̃^�C�v
	BulletType m_bulletType;
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �J�����̎��_ 
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// �J�����̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �{�X�̃x�[�X�N���X
	BossBase* m_pBossBase;
};