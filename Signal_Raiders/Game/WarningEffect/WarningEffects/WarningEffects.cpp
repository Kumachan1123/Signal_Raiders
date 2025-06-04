/*
*	@file WarningEffects.cpp
*	@brief ダメージエフェクトや警告エフェクトの管理クラスのソースファイル
*/
#include <pch.h>
#include "WarningEffects.h"
/*
*	@brief コンストラクタ
*	@param resources 共通リソース
*	@return なし
*/
WarningEffects::WarningEffects(CommonResources* resources)
	: m_pCommonResources(resources)// 共通リソース
	, m_warningEffectCount{ 0 }// 攻撃しようとしている敵の数
	, m_pPlayer{}// プレイヤー
	, m_pEnemyManager{}// 敵マネージャー
{}
/*
*	@brief デストラクタ
*	@details ダメージエフェクトや警告エフェクトの管理クラスのデストラクタ
*	@param なし
*	@return なし
*/
WarningEffects::~WarningEffects() {/*do nothing*/ }
/*
*	@brief 初期化
*	@details ダメージエフェクトや警告エフェクトの管理クラスの初期化
*	@param pPlayer プレイヤーのポインタ
*	@param pEnemyManager 敵のポインタ
*	@return なし
*/
void WarningEffects::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	// プレイヤーのポインターを受け取る
	m_pPlayer = pPlayer;
	// 敵のポインターを受け取る
	m_pEnemyManager = pEnemyManager;
}
/*
*	@brief ダメージを受けた時の演出を生成
*	@details ダメージエフェクトを生成する
*	@param なし
*	@return なし
*/
void WarningEffects::CreateDamageEffects()
{
	// ダメージエフェクトの生成
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_pCommonResources);
	// プレイヤーのポインタを設定
	damageEffect->SetPlayer(m_pPlayer);
	// エフェクトタイプを設定
	damageEffect->SetEffectType(DamageEffect::EffectType::DAMAGE);
	// 初期化
	damageEffect->Initialize();
	// ダメージエフェクトをリストに追加
	m_pDamageEffect.push_back(std::move(damageEffect));
}
/*
*	@brief 敵が攻撃してきた時の演出を生成
*	@details 敵が攻撃してきた時の演出を生成する
*	@param なし
*	@return なし
*/
void WarningEffects::CreateInComingEnemy()
{
	// 攻撃してこようとしている敵の数処理
	for (auto& attackingEnemy : m_pEnemyManager->GetEnemies())
	{
		// 攻撃フラグが立っていなかったら次のループへ
		if (!attackingEnemy->GetIsAttack())continue;
		// すでに生成されていたら次のループへ
		if (m_enemyEffectMap.find(attackingEnemy.get()) != m_enemyEffectMap.end())continue;
		// ダメージエフェクトの生成
		std::unique_ptr<DamageEffect> warningEffect = std::make_unique<DamageEffect>(m_pCommonResources);
		// プレイヤーのポインタを設定
		warningEffect->SetPlayer(m_pPlayer);
		// エフェクトタイプを設定
		warningEffect->SetEffectType(DamageEffect::EffectType::INCOMINGENEMY);
		// 攻撃してきた敵のポインタを設定
		warningEffect->SetEnemy(attackingEnemy.get());
		// 初期化
		warningEffect->Initialize();
		// 攻撃フラグをfalseにする
		attackingEnemy->SetIsAttack(false);
		// 敵のポインタを取得
		IEnemy* enemy = attackingEnemy.get();
		// 攻撃してきた敵をリストに追加
		m_pAttackingEnemy.push_back(enemy);
		// m_pDamageEffectに入れた後にm_enemyEffectMapに登録
		m_enemyEffectMap[enemy] = warningEffect.get();
		// まずエフェクトをリストに追加
		m_pDamageEffect.push_back(std::move(warningEffect));
	}
}
/*
*	@brief 更新
*	@details ダメージエフェクトや警告エフェクトの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void WarningEffects::Update(float elapsedTime)
{
	// 新しいダメージエフェクト
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;
	// エフェクトの数更新
	for (auto& damageEffect : m_pDamageEffect)
	{
		// ダメージエフェクトを更新
		damageEffect->Update(elapsedTime);
		// ダメージエフェクトの破棄フラグがtrueだったら
		if (damageEffect->Destroy())
		{
			// ダメージエフェクトを破棄
			damageEffect.reset();
			// 次のループへ
			continue;
		}
		// 新しいリストにダメージエフェクトを追加
		newDamageEffect.push_back(std::move(damageEffect));
	}
	// ダメージエフェクトを新しいリストに置き換える
	m_pDamageEffect = std::move(newDamageEffect);
	// 再生され終わった警告エフェクトと敵のポインターを削除
	for (auto& attackingEnemy : m_pAttackingEnemy)
	{
		// 生成されていなかったら次のループへ
		if (m_enemyEffectMap.find(attackingEnemy) == m_enemyEffectMap.end())continue;
		// 再生フラグがfalseだったら
		if (!m_enemyEffectMap[attackingEnemy]->GetPlayEffect())
		{
			// マップから削除
			m_enemyEffectMap.erase(attackingEnemy);
			// リストから削除
			m_pAttackingEnemy.erase(std::remove(m_pAttackingEnemy.begin(), m_pAttackingEnemy.end(), attackingEnemy), m_pAttackingEnemy.end());
		}
	}
}
/*
*	@brief 描画
*	@details ダメージエフェクトや警告エフェクトの描画
*	@param なし
*	@return なし
*/
void WarningEffects::Render()
{
	// ダメージエフェクトを更新する
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();
}