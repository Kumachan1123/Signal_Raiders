/*
*	@file	DrawCollision.cpp
*	@brief	当たり判定描画用クラス
*/
#include <pch.h>
#include "DrawCollision.h"
// 静的メンバーの初期化
// ベーシックエフェクト
std::unique_ptr<DirectX::BasicEffect> DrawCollision::m_pBasicEffect = nullptr;
// 入力レイアウト
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawCollision::m_pInputLayout = nullptr;
// プリミティブバッチ
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> DrawCollision::m_pPrimitiveBatch = nullptr;
// 共通リソース
CommonResources* DrawCollision::m_pCommonResources = nullptr;
/*
*	@brief 初期化
*	@details 当たり判定描画用クラスの初期化を行う
*	@param commonResources 共通リソース
*	@return なし
*/
void DrawCollision::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定
	m_pCommonResources = commonResources;
	// デバイスを取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ベーシックエフェクトを作成する
	m_pBasicEffect = std::make_unique<BasicEffect>(device);
	// 頂点カラーを有効にする
	m_pBasicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトを作成する
	DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColor>(device, m_pBasicEffect.get(), m_pInputLayout.ReleaseAndGetAddressOf()));
	// プリミティブバッチを作成する
	m_pPrimitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
}
/*
*	@brief 描画開始
*	@details 描画開始時に呼び出す関数
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void DrawCollision::DrawStart(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// 各パラメータを設定する
	// ブレンドステートを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	// 深度ステンシルステートを設定する
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	// ラスタライザーステートを設定する
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_pInputLayout.Get());
	// 入力レイアウトを設定する
	// ベーシックエフェクトを設定する
	// ビュー行列を設定する
	m_pBasicEffect->SetView(view);
	// プロジェクション行列を設定する
	m_pBasicEffect->SetProjection(proj);
	// デバイスコンテキストに適用する
	m_pBasicEffect->Apply(context);
	// 描画開始
	m_pPrimitiveBatch->Begin();
}
/*
*	@brief 境界球描画
*	@details 境界球を描画する
*	@param bs 境界球
*	@param color 境界球の色
*	@return なし
*/
void DrawCollision::DrawBoundingSphere(const DirectX::BoundingSphere& bs, const DirectX::XMVECTOR& color)
{
	// 境界球を描画
	DX::Draw(m_pPrimitiveBatch.get(), bs, color);
}
/*
*	@brief 描画終了
*	@details 描画終了時に呼び出す関数
*	@param なし
*	@return なし
*/
void DrawCollision::DrawEnd()
{
	// 描画終了
	m_pPrimitiveBatch->End();
}
/*
*	@brief 終了処理
*	@details 静的メンバーを解放する
*	@param なし
*	@return なし
*/
void DrawCollision::Finalize()
{
	// 静的メンバーを解放
	// ベーシックエフェクトを解放
	m_pBasicEffect.reset();
	// 入力レイアウトを解放
	m_pInputLayout.Reset();
	// プリミティブバッチを解放
	m_pPrimitiveBatch.reset();
	// 共通リソースを解放
	m_pCommonResources = nullptr;
}
