/*
	@file	BossModel.h
	@brief	敵モデルクラス
*/
#pragma once
#ifndef BOSS_MODEL_DEFINED
#define BOSS_MODEL_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/Microsoft/ReadData.h"
// 自作ヘッダーファイル
#include "Game/Enemy/Boss/Boss.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Interface/IState.h"
#include "Game/Interface/IModel.h"

// クラスの前方宣言
class Boss;
class CommonResources;
class IState;

class BossModel : public IModel
{
public:
	// publicメンバ関数
	BossModel();// コンストラクタ
	~BossModel()override;// デストラクタ
	void Initialize(CommonResources* resources)override;// 初期化
	void SetState(IState::EnemyState State) { m_nowState = State; }// ステートの設定
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj)override;

private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 胴体
	std::unique_ptr<DirectX::Model> m_bodyModel;
	// 攻撃時の顔
	std::unique_ptr<DirectX::Model> m_attackFaceModel;
	// ノックバック中の顔
	std::unique_ptr<DirectX::Model> m_damageFaceModel;
	// 怒り状態の顔
	std::unique_ptr<DirectX::Model> m_angryFaceModel;
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// 現在のステート
	IState::EnemyState m_nowState;
};
#endif //BOSS_MODEL_DEFINED