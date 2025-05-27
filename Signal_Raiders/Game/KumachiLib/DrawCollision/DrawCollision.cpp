/*
*	@file	DrawCollision.cpp
*	@brief	当たり判定描画用クラス
*/
#include <pch.h>
#include "DrawCollision.h"


// 静的メンバーの初期化
std::unique_ptr<DirectX::BasicEffect> DrawCollision::m_pBasicEffect = nullptr;// ベーシックエフェクト
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawCollision::m_pInputLayout = nullptr;// 入力レイアウト
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> DrawCollision::m_pPrimitiveBatch = nullptr;// プリミティブバッチ
CommonResources* DrawCollision::m_pCommonResources = nullptr;// 共通リソース

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
	m_pCommonResources = commonResources;// 共通リソースを設定
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	m_pBasicEffect = std::make_unique<BasicEffect>(device);// ベーシックエフェクトを作成する
	m_pBasicEffect->SetVertexColorEnabled(true);// 頂点カラーを有効にする
	DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColor>	// 入力レイアウトを作成する
		(device, m_pBasicEffect.get(), m_pInputLayout.ReleaseAndGetAddressOf()));
	m_pPrimitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);	// プリミティブバッチを作成する
}
/*
*	@brief 描画開始
*	@details 描画開始時に呼び出す関数
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void DrawCollision::DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	auto states = m_pCommonResources->GetCommonStates();// コモンステートを取得
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);// ブレンドステートを設定する
	context->OMSetDepthStencilState(states->DepthRead(), 0);// 深度ステンシルステートを設定する
	context->RSSetState(states->CullNone());// ラスタライザーステートを設定する
	context->IASetInputLayout(m_pInputLayout.Get());// 入力レイアウトを設定する
	// ベーシックエフェクトを設定する
	m_pBasicEffect->SetView(view);// ビュー行列を設定する
	m_pBasicEffect->SetProjection(proj);// プロジェクション行列を設定する
	m_pBasicEffect->Apply(context);// デバイスコンテキストに適用する
	m_pPrimitiveBatch->Begin();// 描画開始
}
/*
*	@brief 境界球描画
*	@details 境界球を描画する
*	@param bs 境界球
*	@param color 境界球の色
*	@return なし
*/
void DrawCollision::DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color)
{
	DX::Draw(m_pPrimitiveBatch.get(), bs, color);// 境界球を描画
}
/*
*	@brief 描画終了
*	@details 描画終了時に呼び出す関数
*	@param なし
*	@return なし
*/
void DrawCollision::DrawEnd()
{
	m_pPrimitiveBatch->End();// 描画終了
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
	m_pBasicEffect.reset();// ベーシックエフェクトを解放
	m_pInputLayout.Reset();// 入力レイアウトを解放
	m_pPrimitiveBatch.reset();// プリミティブバッチを解放
	m_pCommonResources = nullptr;// 共通リソースを解放
}
