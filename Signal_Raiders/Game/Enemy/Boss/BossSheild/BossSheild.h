/*
*	@file	BossSheild.h
*	@brief	ボスシールドクラス
*/
#pragma once
#ifndef BOSS_SHEILD_DEFINED
#define BOSS_SHEILD_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IState.h"
#include "Game/Particle/Particle.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/BossBase/BossBase.h"
// クラスの前方宣言
class IEnemy;
class Player;
class CommonResources;
class IState;
class BossBase;

class BossSheild
{
public:
	// ボスの種類
	enum class BossShieldType
	{
		BOSS = 0,	// ボス
		LASTBOSS,	// ラスボス
	};
public:
	// アクセサ
	bool GetSheild() const { return m_isSheild; }// シールド展開フラグ取得
	void SetSheild(bool isSheild) { m_isSheild = isSheild; }// シールド展開フラグ設定
	int GetSheildHP() const { return m_sheildHP; }// シールドのHP取得
	void SetSheildHP(int sheildHP) { m_sheildHP = sheildHP; }// シールドのHP設定
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_sheildPosition = pos; }// シールドの座標設定
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_sheildRotation = rot; }// シールドの回転設定
	void SetUp(int sheildHP, IEnemy* pBoss);// シールドの初期化

public:
	// public関数
	BossSheild();// コンストラクタ
	~BossSheild();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void Update(float elapsedTime);// 更新
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// シールドモデル
	DirectX::Model* m_pSheildModel;
	// パーティクル
	std::unique_ptr<Particle> m_pParticle;
	// ボスポインター
	IEnemy* m_pBoss;
	// ボスの種類
	BossShieldType m_bossType;
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
