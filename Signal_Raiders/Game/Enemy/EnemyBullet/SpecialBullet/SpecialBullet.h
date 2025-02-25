#pragma once
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
#include "Game/KumachiLib/KumachiLib.h"
class SpecialBullet : public IEnemyBullet
{
public:// �֐�
	SpecialBullet();
	~SpecialBullet();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
private:
	// ��]�e��W�J����
	void Expand();
	// ��]�e�𔭎˂���
	void Shot();
	// ��]�e�̓W�J����߂�
	void StopExpand();
	// ��]�e�������̎���ɖ߂�
	void ComeBack();
private:// �ϐ�
	EnemyBullet* m_pEnemyBullet;					// �G�e�|�C���^�[
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_target;			// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_currentTarget;	// ���݂̃^�[�Q�b�g�̈ʒu
	DirectX::BoundingSphere m_boundingSphere;		// �u�e�v���E��
	float m_spiralAngle;							// ��]�e�̊p�x
	float m_elapsedTime;							// �o�ߎ���
	float m_time;									// ����
	DirectX::SimpleMath::Vector3 m_basePos;			// �e�̊�ʒu
	float m_rotationSpeed;							// �e�̉�]���x
	float m_distance;								// �e�̓G�Ƃ̋���
	float m_height;									// �e�̍���
	DirectX::SimpleMath::Vector3 m_positionOffSet;	// �e�̈ʒu�I�t�Z�b�g

};