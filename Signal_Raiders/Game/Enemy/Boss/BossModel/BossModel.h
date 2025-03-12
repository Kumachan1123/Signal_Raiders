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

// クラスの前方宣言
class Boss;
class CommonResources;
class IState;


class BossModel
{
public:
	// publicメンバ関数
	BossModel();// コンストラクタ
	~BossModel();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void SetState(IState::EnemyState State) { m_nowState = State; }// ステートの設定
	void Render(ID3D11DeviceContext1* context,// 描画
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);

private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 胴体
	std::unique_ptr<DirectX::Model> m_bodyModel;
	// 攻撃時の顔
	std::unique_ptr<DirectX::Model> m_attackFaceModel;
	// 普段の顔
	std::unique_ptr<DirectX::Model> m_idlingFaceModel;
	// シールド
	std::unique_ptr < DirectX::Model> m_sheildModel;
	// モデルの影用のピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シールドのサイズ
	DirectX::SimpleMath::Vector3 m_sheildSize;
	// シールドの位置
	DirectX::SimpleMath::Vector3 m_sheildPosition;
	// シールドの回転
	DirectX::SimpleMath::Quaternion m_sheildRotation;
	// 現在のステート
	IState::EnemyState m_nowState;
	// シールド展開フラグ
	bool m_isSheild;
};
#endif //BOSS_MODEL_DEFINED