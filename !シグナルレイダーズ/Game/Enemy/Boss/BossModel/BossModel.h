/*
	@file	BossModel.h
	@brief	敵モデルクラス
	作成者：くまち
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class Enemy;
class CommonResources;
class IState;
class BossModel
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// モデル
	std::unique_ptr<DirectX::Model> m_bodyModel;//胴体

	std::unique_ptr<DirectX::Model> m_attackFaceModel;//攻撃時の顔
	std::unique_ptr<DirectX::Model> m_idlingFaceModel;//普段の顔

	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::SimpleMath::Vector3 m_accele;		// 加速度
	DirectX::SimpleMath::Vector3 m_nowScale;		// 現在スケール
	DirectX::SimpleMath::Vector3 m_startScale;		// 初期スケール
	DirectX::SimpleMath::Vector3 m_endScale;		// 最終スケール
	DirectX::SimpleMath::Quaternion m_rotation; // クォータニオン (追加)
	IState::EnemyState m_nowState;
public:
	// 初期ステータスを設定
	BossModel();
	~BossModel();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, IState::EnemyState State);
	void Render(ID3D11DeviceContext1* context,
				DirectX::DX11::CommonStates* states,
				DirectX::SimpleMath::Matrix world,
				DirectX::SimpleMath::Matrix view,
				DirectX::SimpleMath::Matrix proj);

};
#endif //BOSS_MODEL_DEFINED