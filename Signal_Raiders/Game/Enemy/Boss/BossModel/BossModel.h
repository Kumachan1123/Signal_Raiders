/*
	@file	BossModel.h
	@brief	敵モデルクラス
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
class Boss;
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

	std::unique_ptr < DirectX::Model> m_sheildModel;//シールド
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	DirectX::SimpleMath::Vector3 m_sheildSize;
	DirectX::SimpleMath::Vector3 m_sheildPosition;
	DirectX::SimpleMath::Quaternion m_sheildRotation;
	IState::EnemyState m_nowState;
	// シールド展開フラグ
	bool m_isSheild;
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