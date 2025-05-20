/*
	@file	EnemyModel.h
	@brief	敵モデルクラス
*/
#pragma once
#ifndef ENEMY_MODEL_DEFINED
#define ENEMY_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <unordered_map>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"
// 前方宣言
class Enemy;
class CommonResources;
class IState;
class EnemyModel : public IModel
{
public:
	// public関数
	EnemyModel();// コンストラクタ
	~EnemyModel()override;	// デストラクタ
	void Initialize(CommonResources* resources)override;//　初期化
	void SetState(IState::EnemyState State)override { m_nowState = State; }// 状態をセット
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;
private:
	// private変数
	// 共通リソース
	CommonResources* m_commonResources;
	// モデル
	std::unique_ptr<DirectX::Model> m_bodyModel;//胴体
	std::unique_ptr<DirectX::Model> m_antennaModel;//アンテナ
	std::unique_ptr<DirectX::Model> m_handModel;//手
	std::unordered_map<IState::EnemyState, std::unique_ptr<DirectX::Model>> m_faceModelMap;// 表情によって変わる顔のモデルのマップ
	std::unique_ptr<DirectX::Model> m_shadowModel;	// 影用のモデル
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// 現在のステータス
	IState::EnemyState m_nowState;

};
#endif //ENEMY_MODEL_DEFINED