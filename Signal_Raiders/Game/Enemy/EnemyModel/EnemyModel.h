/*
*	@file	EnemyModel.h
*	@brief	敵モデルクラス
*/
#pragma once
#ifndef ENEMY_MODEL_DEFINED
#define ENEMY_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// 前方宣言
class Enemy;
class CommonResources;
class IState;

// 敵モデルクラス
class EnemyModel : public IModel
{
public:
	// public関数
	// コンストラクタ
	EnemyModel();
	// デストラクタ
	~EnemyModel()override;
	// 初期化
	void Initialize(CommonResources* resources)override;
	// 状態をセット
	void SetState(IState::EnemyState State)override { m_nowState = State; }
	// 描画
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj)override;
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 頭モデル
	DirectX::Model* m_pBodyModel;
	// アンテナモデル
	DirectX::Model* m_pAntennaModel;
	// 手モデル
	DirectX::Model* m_pHandModel;
	// 表情によって変わる顔のモデルのマップ
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// 影用のモデル
	DirectX::Model* m_pShadowModel;
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// 現在のステータス
	IState::EnemyState m_nowState;
};
#endif //ENEMY_MODEL_DEFINED