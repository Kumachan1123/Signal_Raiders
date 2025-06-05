/*
	@file	VerticalAttackerModel.h
	@brief	垂直攻撃敵モデルクラス
*/
#pragma once
#ifndef VERTICAL_ATTACKER_MODEL_DEFINED
#define VERTICAL_ATTACKER_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <unordered_map>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// 前方宣言
class VerticalAttacker;
class CommonResources;
class IState;

// 垂直攻撃敵モデルクラス
class VerticalAttackerModel : public IModel
{
public:
	// public関数
	// コンストラクタ
	VerticalAttackerModel();
	// デストラクタ
	~VerticalAttackerModel()override;
	// 初期化
	void Initialize(CommonResources* resources)override;
	// 状態設定
	void SetState(IState::EnemyState State)override { m_nowState = State; }
	// 描画
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj)	override;
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 胴体モデル
	DirectX::Model* m_pBodyModel;
	// 表情によって変わる顔のモデルのマップ
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// 現在のステータス
	IState::EnemyState m_nowState;
};
#endif //VERTICAL_ATTACKER_MODEL_DEFINED