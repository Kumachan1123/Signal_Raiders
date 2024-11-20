/*
	@file	BossSheild.h
	@brief	ボスシールドクラス
*/
#pragma once
#ifndef BOSS_SHEILD_DEFINED
#define BOSS_SHEILD_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
#include "Game/Particle/Particle.h"
#include "Game/Enemy/Boss/Boss.h"
class Boss;
class Player;
class CommonResources;
class IState;
class BossSheild
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// シールドモデル
	std::unique_ptr < DirectX::Model> m_sheildModel;
	std::unique_ptr<Particle> m_pParticle;// パーティクル
	Boss* m_pBoss;// ボス
	DirectX::SimpleMath::Vector3 m_sheildSize;// シールドのサイズ
	DirectX::SimpleMath::Vector3 m_sheildPosition;// シールドの座標
	DirectX::SimpleMath::Quaternion m_sheildRotation;// シールドの回転
	// シールド展開フラグ
	bool m_isSheild;
	// パーティクル再生可能フラグ
	bool m_isParticle;
	// シールドのHP
	int m_sheildHP;

public:
	// 初期ステータスを設定
	BossSheild(int sheildHP, Boss* pBoss);
	~BossSheild();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
public:
	// getter
	bool GetSheild() const { return m_isSheild; }
	int GetSheildHP() const { return m_sheildHP; }
	// setter
	void SetSheildHP(int sheildHP) { m_sheildHP = sheildHP; }
	void SetSheild(bool isSheild) { m_isSheild = isSheild; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_sheildPosition = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_sheildRotation = rot; }
};
#endif //BOSS_SHEILD_DEFINED
