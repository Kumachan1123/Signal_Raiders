#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
class Player;
class CommonResources;
class BulletManager;

class IBossLogic
{
public:
	virtual ~IBossLogic() = default;                                          // �f�X�g���N�^
	//virtual void SetPlayer(Player* pPlayer) = 0;                               // �v���C���[�ݒ�
	//virtual void SetEnemyHP(int hp) = 0;                                    // HP�ݒ�
	//virtual void SetAudioManager(class AudioManager* audioManager) = 0;        // �I�[�f�B�I�}�l�[�W���[�ݒ�
	//virtual void SetBulletManager(BulletManager* bulletManager) = 0;          // �e�}�l�[�W���[�ݒ�
	//virtual void SetBulletType(int bossBulletType) = 0;                        // �e�^�C�v�ݒ�
	virtual void Initialize() = 0;                                             // ������
	virtual void Update(float elapsedTime) = 0;                                // �X�V
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;        // �`��
	virtual void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0; // �Փ˔���`��
};
#endif // IBOSSLOGIC_DEFINED
