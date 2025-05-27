/*
*	@file	LastBossModel.h
*	@brief	ラスボスモデルクラス
*/
#pragma once
#ifndef LASTBOSS_MODEL_DEFINED
#define LASTBOSS_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// クラスの前方宣言
class Boss;
class CommonResources;
class IState;

class LastBossModel : public IModel
{


public:
	// publicメンバ関数
	LastBossModel();// コンストラクタ
	~LastBossModel()override;// デストラクタ
	void Initialize(CommonResources* resources)override;// 初期化
	void SetState(IState::EnemyState State) { m_nowState = State; }// ステートの設定
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;

private:
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 胴体
	DirectX::Model* m_pBodyModel;
	// 表情によって変わる顔のモデルのマップ
	std::unordered_map<IState::EnemyState, DirectX::Model*> m_pFaceModelMap;
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// 現在のステート
	IState::EnemyState m_nowState;
};
#endif //LASTBOSS_MODEL_DEFINED