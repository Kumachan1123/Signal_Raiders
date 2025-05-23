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
	: m_commonResources(resources)// 共通リソース
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
	m_pPlayer = pPlayer;// プレイヤーのポインターを受け取る
	m_pEnemyManager = pEnemyManager;// 敵のポインターを受け取る
}

/*
*	@brief ダメージを受けた時の演出を生成
*	@details ダメージエフェクトを生成する
*	@param なし
*	@return なし
*/
void WarningEffects::CreateDamageEffects()
{
	std::unique_ptr<DamageEffect> damageEffect = std::make_unique<DamageEffect>(m_commonResources);// ダメージエフェクトの生成
	damageEffect->SetPlayer(m_pPlayer);// プレイヤーのポインタを設定
	damageEffect->SetEffectType(DamageEffect::EffectType::DAMAGE);// エフェクトタイプを設定
	damageEffect->Initialize();// 初期化
	m_pDamageEffect.push_back(std::move(damageEffect));// ダメージエフェクトをリストに追加
}

/*
*	@brief 敵が攻撃してきた時の演出を生成
*	@details 敵が攻撃してきた時の演出を生成する
*	@param なし
*	@return なし
*/
void WarningEffects::CreateInComingEnemy()
{
	for (auto& attackingEnemy : m_pEnemyManager->GetEnemies())// 攻撃してこようとしている敵の数処理
	{
		if (!attackingEnemy->GetIsAttack())continue;// 攻撃フラグが立っていなかったら次のループへ
		if (m_enemyEffectMap.find(attackingEnemy.get()) != m_enemyEffectMap.end())continue;// すでに生成されていたら次のループへ
		std::unique_ptr<DamageEffect> warningEffect = std::make_unique<DamageEffect>(m_commonResources);// ダメージエフェクトの生成
		warningEffect->SetPlayer(m_pPlayer);// プレイヤーのポインタを設定
		warningEffect->SetEffectType(DamageEffect::EffectType::INCOMINGENEMY);// エフェクトタイプを設定
		warningEffect->SetEnemy(attackingEnemy.get());// 攻撃してきた敵のポインタを設定
		warningEffect->Initialize();// 初期化
		attackingEnemy->SetIsAttack(false);// 攻撃フラグをfalseにする
		IEnemy* enemy = attackingEnemy.get();// 敵のポインタを取得
		m_pAttackingEnemy.push_back(enemy); // 攻撃してきた敵をリストに追加
		m_enemyEffectMap[enemy] = warningEffect.get();// m_pDamageEffectに入れた後にm_enemyEffectMapに登録
		m_pDamageEffect.push_back(std::move(warningEffect));// まずエフェクトをリストに追加
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
	std::vector<std::unique_ptr<DamageEffect>> newDamageEffect;// 新しいダメージエフェクト
	for (auto& damageEffect : m_pDamageEffect)// エフェクトの数更新
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
	for (auto& attackingEnemy : m_pAttackingEnemy)	// 再生され終わった警告エフェクトと敵のポインターを削除
	{
		if (m_enemyEffectMap.find(attackingEnemy) == m_enemyEffectMap.end())continue;// 生成されていなかったら次のループへ
		if (!m_enemyEffectMap[attackingEnemy]->GetPlayEffect())// 再生フラグがfalseだったら
		{
			m_enemyEffectMap.erase(attackingEnemy);// マップから削除
			m_pAttackingEnemy.erase(std::remove(m_pAttackingEnemy.begin(), m_pAttackingEnemy.end(), attackingEnemy), m_pAttackingEnemy.end());// リストから削除
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
	for (auto& damageEffect : m_pDamageEffect)if (damageEffect->GetPlayEffect())damageEffect->Render();	// ダメージエフェクトを更新する
}
