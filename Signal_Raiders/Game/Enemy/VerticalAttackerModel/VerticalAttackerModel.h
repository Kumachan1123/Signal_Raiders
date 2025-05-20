/*
	@file	VerticalAttackerModel.h
	@brief	範囲攻撃敵モデルクラス
*/
#pragma once
#ifndef AREA_ATTACKER_MODEL_DEFINED
#define AREA_ATTACKER_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <unordered_map>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// 自作ヘッダーファイル
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"
// 前方宣言
class VerticalAttacker;
class CommonResources;
class IState;
class VerticalAttackerModel : public IModel
{
public:
	// public関数
	VerticalAttackerModel();// コンストラクタ
	~VerticalAttackerModel()override;// デストラクタ
	void Initialize(CommonResources* resources)override;// 初期化
	void SetState(IState::EnemyState State)override { m_nowState = State; }// 状態設定
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;
private:
	// private変数
	CommonResources* m_commonResources;	// 共通リソース
	// モデル
	std::unique_ptr<DirectX::Model> m_bodyModel;//胴体
	std::unordered_map<IState::EnemyState, std::unique_ptr<DirectX::Model>> m_faceModelMap;// 表情によって変わる顔のモデルのマップ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;	// モデルの影用のピクセルシェーダー
	IState::EnemyState m_nowState;	// 現在のステータス
};
#endif //AREA_ATTACKER_MODEL_DEFINED