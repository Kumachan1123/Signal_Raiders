/*
*	@file DamageEffects.h
*	@brief ダメージエフェクトの管理クラスのヘッダーファイル
*/
#pragma once
// 標準ライブラリ
#include <vector>
// 外部ライブラリ
#include "Game/CommonResources.h"
// 自作ヘッダーファイル
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/DamageEffect/DamageEffect/DamageEffect.h"

// クラスの前方宣言
class Player;
class EnemyManager;
class CommonResources;
class DamageEffect;


class DamageEffects
{
public:
	// publicメンバ関数
	DamageEffects(CommonResources* resources);// コンストラクタ
	~DamageEffects();// デストラクタ

	void Create();	// ダメージを受けた時の演出を生成
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);// 初期化
	void Update(float elapsedTime);// 更新
	void Render();// 描画
private:
	CommonResources* m_commonResources;// 共通リソース
	Player* m_pPlayer;// プレイヤー
	EnemyManager* m_pEnemyManager;// 敵
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;	// ダメージエフェクト
};