/*
*  @file LastBoss.h
*  @brief ���X�{�X�N���X
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
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
//#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
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
public:
	// �����X�e�[�^�X��ݒ�
	LastBoss(BossBase* pBoss, CommonResources* commonResources);
	~LastBoss()override;
	inline void CreateModel()override;                                             // Boss, LastBoss�ɂă��f���𐶐�
	inline void ChangeState()override;                                // Boss,LastBoss�ɂď�Ԃ�ύX
	inline void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;        // Boss, LastBoss�ɂĕ`��

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	std::unique_ptr<LastBossModel>		m_pBossModel;//���f��
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HP�o�[
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::BoundingSphere m_bossBS;	//�G�̋��E��
	DirectX::SimpleMath::Matrix m_matrix;// �}�g���N�X
	// �e�̔��ˈʒu
	// ����
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// ��
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;
	// �E
	DirectX::SimpleMath::Vector3 m_bulletPosRight;
	// ���ˈʒu����]�����邽�߂̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// �e�̃^�C�v
	EnemyBullet::BulletType m_bulletType;// EnemyBullet�N���X�ɑ���
	// �e�̔�ԕ���
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// ����
	float m_time;
	// �v���C���[�̃J�����̏��
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	BossBase* m_pBossBase;

};