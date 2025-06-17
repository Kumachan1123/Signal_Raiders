/*
	@file	CowardlyEnemyModel.h
	@brief	すばしっこく卑怯な敵モデルクラス
*/
#pragma once
#ifndef  COWARDLY_ENEMY_MODEL_DEFINED
#define  COWARDLY_ENEMY_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <unordered_map>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/CowardlyEnemy/CowardlyEnemy.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// 前方宣言
class VerticalAttacker;
class CommonResources;
class IState;

// すばしっこくて卑怯な敵モデルクラス
class CowardlyEnemyModel : public IModel
{
public:
	// public関数
	// コンストラクタ
	CowardlyEnemyModel();
	// デストラクタ
	~CowardlyEnemyModel()override;
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
	// シェーダー作成クラスのポインター
	CreateShader* m_pCreateShader;
	// 胴体モデル
	DirectX::Model* m_pBodyModel;
	// 表情によって変わる顔のモデルのマップ
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// 現在のステータス
	IState::EnemyState m_nowState;
};
#endif // COWARDLY_ENEMY_MODEL_DEFINED