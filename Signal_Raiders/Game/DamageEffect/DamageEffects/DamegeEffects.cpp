#include <pch.h>
#include "DamageEffects.h"

// コンストラクタ
DamageEffects::DamageEffects(CommonResources* resources)
	: m_commonResources{ resources }
	, m_pPlayer{}
{}

// デストラクタ
DamageEffects::~DamageEffects()
{
}

// 初期化
void DamageEffects::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}


// ダメージを受けた時の演出を生成
void DamageEffects::Create(DX::DeviceResources* pDR)
{
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_commonResources);
	damageEffect->Initialize(m_pPlayer);
	m_pDamageEffect.push_back(std::move(damageEffect));

}

// 更新
void DamageEffects::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;// 新しいダメージエフェクト
	for (auto& damageEffect : m_pDamageEffect)
	{
		damageEffect->Update(elapsedTime);
		// 再生が終わったダメージエフェクトだったらそのエフェクトを完全に破棄
		if (damageEffect->Destroy())
		{
			damageEffect.reset();
			continue;
		}
		// 再生が終了していないエフェクトは新しいリストに移動
		newDamageEffect.push_back(std::move(damageEffect));
	}
	// ダメージエフェクトを新しいリストに置き換える
	m_pDamageEffect = std::move(newDamageEffect);
}

void DamageEffects::Render()
{
	// ダメージエフェクトを更新する
	for (auto& damageEffect : m_pDamageEffect)
	{
		if (damageEffect->GetPlayEffect())damageEffect->Render();
	}
}
