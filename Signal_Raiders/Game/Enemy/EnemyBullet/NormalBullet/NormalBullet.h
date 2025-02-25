#pragma once
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
class NormalBullet : public IEnemyBullet
{
public:// �֐�
	NormalBullet();
	~NormalBullet();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
private:// �ϐ�
	EnemyBullet* m_pEnemyBullet;					// �G�e�|�C���^�[
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	DirectX::SimpleMath::Vector3 m_target;			// �e���ˎ��̃^�[�Q�b�g�̈ʒu
	DirectX::BoundingSphere m_boundingSphere;		// �u�e�v���E��
};