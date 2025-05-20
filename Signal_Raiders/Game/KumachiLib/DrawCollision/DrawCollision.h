/*
	@file	DrawCollision.h
	@brief	当たり判定描画用クラス
*/
#pragma once
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <Libraries/Microsoft/DebugDraw.h>
//前方宣言
class CommonResources;

class DrawCollision
{
public:
	// public関数
	static void Initialize(CommonResources* commonResources);// 初期化
	static void DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 描画開始
	static void DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color);// 境界球描画
	static void DrawEnd();// 描画終了
	static void Finalize();// 解放
private:
	// private変数
	// ベーシックエフェクト
	static std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// プリミティブバッチ
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 共通リソース
	static CommonResources* m_commonResources;
};