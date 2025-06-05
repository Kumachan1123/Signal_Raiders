/*
*	@file	DrawCollision.h
*	@brief	当たり判定描画用クラス
*/
#pragma once
#ifndef DRAWCOLLISION_DEFINED
#define DRAWCOLLISION_DEFINED
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"

//前方宣言
class CommonResources;

// 当たり判定描画用クラス
class DrawCollision
{
public:
	// public関数
	// 初期化
	static void Initialize(CommonResources* commonResources);
	// 描画開始
	static void DrawStart(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 境界球描画
	static void DrawBoundingSphere(const DirectX::BoundingSphere& bs, const DirectX::XMVECTOR& color);
	// 描画終了
	static void DrawEnd();
	// 解放
	static void Finalize();
private:
	// private変数
	// ベーシックエフェクト
	static std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;
	// 入力レイアウト
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// プリミティブバッチ
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_pPrimitiveBatch;
	// 共通リソース
	static CommonResources* m_pCommonResources;
};
#endif // DRAWCOLLISION_DEFINED