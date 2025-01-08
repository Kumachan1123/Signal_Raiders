/*
	@file	EnemyModel.h
	@brief	敵モデルクラス
*/
#pragma once
#ifndef ENEMY_MODEL_DEFINED
#define ENEMY_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class Enemy;
class CommonResources;
class IState;
class EnemyModel
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// モデル
	std::unique_ptr<DirectX::Model> m_bodyModel;//胴体
	std::unique_ptr<DirectX::Model> m_antennaModel;//アンテナ
	std::unique_ptr<DirectX::Model> m_handModel;//手
	std::unique_ptr<DirectX::Model> m_attackFaceModel;//攻撃態勢の顔
	std::unique_ptr<DirectX::Model> m_angryFaceModel;//おこの時の顔
	std::unique_ptr<DirectX::Model> m_idlingFaceModel;//普段の顔
	std::unique_ptr<DirectX::Model> m_damageFaceModel;	// 攻撃を受けた時の顔
	std::unique_ptr<DirectX::Model> m_shadowModel;	// 影用のモデル
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// 現在のステータス
	IState::EnemyState m_nowState;
public:
	// 初期ステータスを設定
	EnemyModel();
	~EnemyModel();
	void Initialize(CommonResources* resources);
	void SetState(IState::EnemyState State) { m_nowState = State; }
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);

};
#endif //ENEMY_MODEL_DEFINED