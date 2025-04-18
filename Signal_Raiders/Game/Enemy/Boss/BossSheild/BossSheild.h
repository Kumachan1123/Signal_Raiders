/*
	@file	BossSheild.h
	@brief	ボスシールドクラス
*/
#pragma once
#ifndef BOSS_SHEILD_DEFINED
#define BOSS_SHEILD_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IState.h"
#include "Game/Particle/Particle.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Enemy/LastBoss/LastBoss.h"

// クラスの前方宣言
class IEnemy;
class Player;
class CommonResources;
class IState;

class BossSheild
{
public:
	// ボスの種類
	enum class BossType
	{
		BOSS = 0,	// ボス
		LASTBOSS,	// ラスボス
	};
public:
	// アクセサ
	bool GetSheild() const { return m_isSheild; }
	void SetSheild(bool isSheild) { m_isSheild = isSheild; }
	int GetSheildHP() const { return m_sheildHP; }
	void SetSheildHP(int sheildHP) { m_sheildHP = sheildHP; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_sheildPosition = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_sheildRotation = rot; }
public:
	// public関数
	BossSheild(BossType type, int sheildHP, IEnemy* pBoss);// コンストラクタ
	~BossSheild();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void Update(float elapsedTime);// 更新
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// シールドモデル
	std::unique_ptr < DirectX::Model> m_sheildModel;
	// パーティクル
	std::unique_ptr<Particle> m_pParticle;
	// ボス
	IEnemy* m_pBoss;
	// ボスの種類
	BossType m_bossType;
	// シールドのサイズ
	DirectX::SimpleMath::Vector3 m_sheildSize;
	// シールドの座標
	DirectX::SimpleMath::Vector3 m_sheildPosition;
	// シールドの回転
	DirectX::SimpleMath::Quaternion m_sheildRotation;
	// シールド展開フラグ
	bool m_isSheild;
	// パーティクル再生可能フラグ
	bool m_isParticle;
	// シールドのHP
	int m_sheildHP;

};
#endif //BOSS_SHEILD_DEFINED
