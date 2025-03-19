/*
*	@file DamageEffects.cpp
*	@brief ダメージエフェクトの管理クラスのソースファイル
*/
#include <pch.h>
#include "DamageEffects.h"

/*
*	@brief コンストラクタ
*	@param[in] resources 共通リソース
*	@return なし
*/
DamageEffects::DamageEffects(CommonResources* resources)
	: m_commonResources(resources)
	, m_pPlayer{}
{}

/*
*	@brief デストラクタ
*	@return なし
*/
DamageEffects::~DamageEffects() {}

/*
*	@brief 初期化
*	@param[in] pPlayer プレイヤーのポインタ
*	@param[in] pEnemyManager 敵のポインタ
*	@return なし
*/
void DamageEffects::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;// プレイヤーのポインターを受け取る
	m_pEnemyManager = pEnemyManager;// 敵のポインターを受け取る
}

/*
*	@brief ダメージを受けた時の演出を生成
*	@return なし
*/
void DamageEffects::Create()
{
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_commonResources);// ダメージエフェクトの生成
	damageEffect->Initialize(m_pPlayer);// 初期化
	m_pDamageEffect.push_back(std::move(damageEffect));// ダメージエフェクトをリストに追加
}

/*
*	@brief 更新
*	@param[in] elapsedTime 経過時間
*	@return なし
*/
void DamageEffects::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;// 新しいダメージエフェクト
	for (auto& damageEffect : m_pDamageEffect)
	{
		damageEffect->Update(elapsedTime);// ダメージエフェクトを更新
		// 再生が終わったダメージエフェクトだったらそのエフェクトを完全に破棄
		if (damageEffect->Destroy())// ダメージエフェクトの破棄フラグがtrueだったら
		{
			damageEffect.reset();// ダメージエフェクトを破棄
			continue;// 次のループへ
		}
		newDamageEffect.push_back(std::move(damageEffect));// 新しいリストにダメージエフェクトを追加
	}
	m_pDamageEffect = std::move(newDamageEffect);// ダメージエフェクトを新しいリストに置き換える
}
/*
*	@brief 描画
*	@return なし
*/
void DamageEffects::Render()
{
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();	// ダメージエフェクトを更新する
}
