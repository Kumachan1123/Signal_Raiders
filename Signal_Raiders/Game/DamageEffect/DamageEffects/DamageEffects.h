// �U�����󂯂����̉��o���Ǘ�����N���X
#pragma once
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"
#include "Game/DamageEffect/DamageEffect/DamageEffect.h"
class Player;
class CommonResources;
class DamageEffect;
class DamageEffects
{
private:
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	Player* m_pPlayer;// �v���C���[
	// �_���[�W�G�t�F�N�g
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;
public:
	DamageEffects(CommonResources* resources);
	~DamageEffects();

	// �_���[�W���󂯂����̉��o�𐶐�
	void Create(DX::DeviceResources* pDR);

	void Initialize(Player* pPlayer);
	void Update(float elapsedTime);

	void Render();

};