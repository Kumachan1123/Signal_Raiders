#include <pch.h>
#include "DamageEffects.h"

// �R���X�g���N�^
DamageEffects::DamageEffects(CommonResources* resources)
	: m_commonResources{ resources }
	, m_pPlayer{}
{}

// �f�X�g���N�^
DamageEffects::~DamageEffects()
{
}

// ������
void DamageEffects::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}


// �_���[�W���󂯂����̉��o�𐶐�
void DamageEffects::Create(DX::DeviceResources* pDR)
{
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_commonResources);
	damageEffect->Initialize(m_pPlayer);
	m_pDamageEffect.push_back(std::move(damageEffect));

}

// �X�V
void DamageEffects::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;// �V�����_���[�W�G�t�F�N�g
	for (auto& damageEffect : m_pDamageEffect)
	{
		damageEffect->Update(elapsedTime);
		// �Đ����I������_���[�W�G�t�F�N�g�������炻�̃G�t�F�N�g�����S�ɔj��
		if (damageEffect->Destroy())
		{
			damageEffect.reset();
			continue;
		}
		// �Đ����I�����Ă��Ȃ��G�t�F�N�g�͐V�������X�g�Ɉړ�
		newDamageEffect.push_back(std::move(damageEffect));
	}
	// �_���[�W�G�t�F�N�g��V�������X�g�ɒu��������
	m_pDamageEffect = std::move(newDamageEffect);
}

void DamageEffects::Render()
{
	// �_���[�W�G�t�F�N�g���X�V����
	for (auto& damageEffect : m_pDamageEffect)
	{
		if (damageEffect->GetPlayEffect())damageEffect->Render();
	}
}
