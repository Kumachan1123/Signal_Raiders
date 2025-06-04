/*
*	@file WarningEffects.h
*	@brief ダメージエフェクトや警告エフェクトの管理クラスのヘッダーファイル
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <unordered_map>
// 外部ライブラリ
#include "Game/CommonResources.h"
// 自作ヘッダーファイル
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/WarningEffect/DamageEffect/DamageEffect.h"
// 前方宣言
class Player;
class EnemyManager;
class CommonResources;
class DamageEffect;
class IEnemy;
class WarningEffects
{
public:	// アクセサ
	// 攻撃しようとしている敵の数取得
	int GetWarningEffectCount()const { return m_warningEffectCount; }
	// 攻撃しようとしている敵の数設定
	void SetWarningEffectCount(int count) { m_warningEffectCount = count; }
public:	// publicメンバ関数
	// コンストラクタ
	WarningEffects(CommonResources* resources);
	// デストラクタ
	~WarningEffects();
	// ダメージを受けた時の演出を生成
	void CreateDamageEffects();
	// 敵が攻撃してきた時の演出を生成
	void CreateInComingEnemy();
	// 初期化
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
private:// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// プレイヤー
	Player* m_pPlayer;
	// 敵
	EnemyManager* m_pEnemyManager;
	// ダメージエフェクト
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;
	// 攻撃してきた敵リスト
	std::vector<IEnemy*> m_pAttackingEnemy;
	// 攻撃しようとしている敵の数
	int m_warningEffectCount;
	// 敵とエフェクトのマップ
	std::unordered_map<IEnemy*, DamageEffect*> m_enemyEffectMap;
};