// 攻撃を受けた時の演出を管理するクラス
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
	CommonResources* m_commonResources;// 共通リソース
	Player* m_pPlayer;// プレイヤー
	// ダメージエフェクト
	std::vector<std::unique_ptr<DamageEffect>> m_pDamageEffect;
public:
	DamageEffects(CommonResources* resources);
	~DamageEffects();

	// ダメージを受けた時の演出を生成
	void Create(DX::DeviceResources* pDR);

	void Initialize(Player* pPlayer);
	void Update(float elapsedTime);

	void Render();

};