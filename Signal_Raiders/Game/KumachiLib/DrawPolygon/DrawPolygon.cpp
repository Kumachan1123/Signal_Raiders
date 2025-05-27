/*
*	@file	DrawPolygon.cpp
*	@brief	板ポリゴン描画用クラス
*	@details 板ポリゴンを描画するためのクラス
*/
#include <pch.h>
#include "DrawPolygon.h"

// シングルトンインスタンスの初期化
std::unique_ptr<DrawPolygon> DrawPolygon::m_pInstance = nullptr;
/*
*	@brief シングルトンインスタンスを取得
*	@details シングルトンインスタンスを取得する
*	@param なし
*	@return シングルトンインスタンス
*/
DrawPolygon* const DrawPolygon::GetInstance()
{
	if (m_pInstance == nullptr)// インスタンスがない場合
	{
		m_pInstance.reset(new DrawPolygon());// インスタンスを生成
	}
	return m_pInstance.get();// インスタンスを返す
}

/*
*	@brief コンストラクタ
*	@details コンストラクタ
*	@param なし
*	@return なし
*/
DrawPolygon::DrawPolygon()
	: m_pPrimitiveBatchTexture(nullptr)// プリミティブバッチ(頂点、テクスチャ)
	, m_pPrimitiveBatchColorTexture(nullptr)// プリミティブバッチ(頂点、色、テクスチャ)
	, m_pStates(nullptr)// コモンステート
	, m_pDeviceContext(nullptr)	// デバイスコンテキスト
	, m_pDR(nullptr)// デバイスリソース
	, m_pDevice(nullptr)// デバイス
{
}
/*
*	@brief デストラクタ
*	@details デストラクタ
*	@param なし
*	@return なし
*/
DrawPolygon::~DrawPolygon()
{
	ReleasePositionTexture();	// プリミティブバッチの解放（頂点、テクスチャ）
	ReleasePositionColorTexture();	// プリミティブバッチの解放（頂点、色、テクスチャ）
}
/*
*	@brief 初期化
*	@details 初期化（頂点、テクスチャ）を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void DrawPolygon::InitializePositionTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pDevice = pDR->GetD3DDevice();// デバイスを取得
	m_pDeviceContext = pDR->GetD3DDeviceContext();// デバイスコンテキストを取得
	m_pPrimitiveBatchTexture = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());// プリミティブバッチを作成する
	m_pStates = std::make_unique<CommonStates>(m_pDevice);//	コモンステートを作成する
}

/*
*	@brief 初期化
*	@details 初期化（頂点、色、テクスチャ）を行う
*	@param pDR デバイスリソース
*	@return なし
*/
void DrawPolygon::InitializePositionColorTexture(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pDevice = pDR->GetD3DDevice();// デバイスを取得
	m_pDeviceContext = pDR->GetD3DDeviceContext();// デバイスコンテキストを取得
	m_pPrimitiveBatchColorTexture = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());// プリミティブバッチの作成
	m_pStates = std::make_unique<CommonStates>(m_pDevice);//	コモンステートを作成する
}
/*
*	@brief 描画開始
*	@details 描画開始時に呼び出す関数
*	@param pInputLayout 入力レイアウト
*	@param textures テクスチャ配列
*	@return なし
*/
void DrawPolygon::DrawStart(ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures)
{
	for (int i = 0; i < textures.size(); i++)// ピクセルシェーダにテクスチャを登録する。
		m_pDeviceContext->PSSetShaderResources(i, 1, textures[i].GetAddressOf());// for文で一気に設定する
	m_pDeviceContext->IASetInputLayout(pInputLayout);// インプットレイアウトの登録
}
/*
*	@brief 描画前設定
*	@details 描画前に各種ステートを設定する
*	@param ss サンプラーステート
*	@param bs ブレンドステート
*	@param rs ラスタライザーステート
*	@param dss 深度ステンシルステート
*	@return なし
*/
void DrawPolygon::DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss)
{
	ID3D11SamplerState* sampler[1];//	画像用サンプラーの登録
	switch (ss)// サンプラーステートの種類を判別して、サンプラーを設定する
	{
	case DrawPolygon::SamplerStates::ANISOTROPIC_CLAMP:// アニソトロピッククランプ
		sampler[0] = m_pStates->AnisotropicClamp();// アニソトロピッククランプのサンプラーを取得
		break;
	case DrawPolygon::SamplerStates::ANISOTROPIC_WRAP:// アニソトロピックラップ
		sampler[0] = m_pStates->AnisotropicWrap(); // アニソトロピックラップのサンプラーを取得
		break;
	case DrawPolygon::SamplerStates::LINEAR_CLAMP: // リニアクランプ
		sampler[0] = m_pStates->LinearClamp(); // リニアクランプのサンプラーを取得
		break;
	case DrawPolygon::SamplerStates::LINEAR_WRAP: // リニアラップ
		sampler[0] = m_pStates->LinearWrap(); // リニアラップのサンプラーを取得
		break;
	case DrawPolygon::SamplerStates::POINT_CLAMP: // ポイントクランプ
		sampler[0] = m_pStates->PointClamp(); // ポイントクランプのサンプラーを取得
		break;
	case DrawPolygon::SamplerStates::POINT_WRAP: // ポイントラップ
		sampler[0] = m_pStates->PointWrap(); // ポイントラップのサンプラーを取得
		break;
	default: // デフォルトはnullptr
		sampler[0] = nullptr; // サンプラーをnullptrに設定
		break;
	}
	m_pDeviceContext->PSSetSamplers(0, 1, sampler);// ピクセルシェーダにサンプラーを登録する
	ID3D11BlendState* blendState;// ブレンドステートの登録
	switch (bs)// ブレンドステートの種類を判別して、ブレンドステートを設定する
	{
	case DrawPolygon::BlendStates::ALPHA:// アルファブレンド
		blendState = m_pStates->AlphaBlend(); // アルファブレンドのブレンドステートを取得
		break;
	case DrawPolygon::BlendStates::ADDITIVE: // 加算ブレンド
		blendState = m_pStates->Additive(); // 加算ブレンドのブレンドステートを取得
		break;
	case DrawPolygon::BlendStates::OPAQUE: // 不透明
		blendState = m_pStates->Opaque(); // 不透明のブレンドステートを取得
		break;
	case DrawPolygon::BlendStates::NONPREMULTIPLIED: // 非プリマルチプライド
		blendState = m_pStates->NonPremultiplied(); // 非プリマルチプライドのブレンドステートを取得
		break;
	default: // デフォルトはnullptr
		blendState = nullptr; // ブレンドステートをnullptrに設定
		break;
	}
	m_pDeviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);// ブレンドステートを設定する

	ID3D11DepthStencilState* depthStencilState;// 深度ステンシルステートの登録
	switch (dss)// 深度ステンシルステートの種類を判別して、深度ステンシルステートを設定する
	{
	case DrawPolygon::DepthStencilStates::DEPTH_DEFAULT:// デフォルト
		depthStencilState = m_pStates->DepthDefault(); // デフォルトの深度ステンシルステートを取得
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_NONE: // 深度なし
		depthStencilState = m_pStates->DepthNone(); // 深度なしの深度ステンシルステートを取得
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ: // 深度読み取り
		depthStencilState = m_pStates->DepthRead(); // 深度読み取りの深度ステンシルステートを取得
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_READ_REVERSE_Z: // 深度読み取り（逆Z）
		depthStencilState = m_pStates->DepthReadReverseZ(); // 深度読み取り（逆Z）の深度ステンシルステートを取得
		break;
	case DrawPolygon::DepthStencilStates::DEPTH_REVERSE_Z: // 逆Z
		depthStencilState = m_pStates->DepthReverseZ(); // 逆Zの深度ステンシルステートを取得
		break;
	default: // デフォルトはnullptr
		depthStencilState = nullptr; // 深度ステンシルステートをnullptrに設定
		break;
	}
	m_pDeviceContext->OMSetDepthStencilState(depthStencilState, 0);// 深度ステンシルステートを設定する

	ID3D11RasterizerState* rasterizerState;// ラスタライザーステートの登録
	switch (rs)// ラスタライザーステートの種類を判別して、ラスタライザーステートを設定する
	{
	case DrawPolygon::RasterizerStates::CULL_CLOCKWISE: // 時計回り
		rasterizerState = m_pStates->CullClockwise(); // 時計回りのラスタライザーステートを取得
		break;
	case DrawPolygon::RasterizerStates::CULL_COUNTERCLOCKWISE: // 反時計回り
		rasterizerState = m_pStates->CullCounterClockwise(); // 反時計回りのラスタライザーステートを取得
		break;
	case DrawPolygon::RasterizerStates::CULL_NONE: // カリングなし
		rasterizerState = m_pStates->CullNone(); // カリングなしのラスタライザーステートを取得
		break;
	case DrawPolygon::RasterizerStates::WIREFRAME: // ワイヤーフレーム
		rasterizerState = m_pStates->Wireframe(); // ワイヤーフレームのラスタライザーステートを取得
		break;
	default: // デフォルトはnullptr
		rasterizerState = nullptr; // ラスタライザーステートをnullptrに設定
		break;
	}
	m_pDeviceContext->RSSetState(rasterizerState); // ラスタライザーステートを設定する

}

/*
*	@brief リソースの更新
*	@details リソースの更新を行う
*	@param resource 更新するリソース
*	@param pSrcData 更新するデータ
*	@return なし
*/
void DrawPolygon::UpdateSubResources(ID3D11Resource* resource, const void* pSrcData)
{
	m_pDeviceContext->UpdateSubresource(resource, 0, NULL, pSrcData, 0, 0);// サブリソースの更新を行う
}

/*
*	@brief 板ポリゴン描画
*	@details 板ポリゴンを描画する(VertexPositionTexture用)
*	@param vertices 頂点情報(VertexPositionTexture)
*	@return なし
*/
void DrawPolygon::DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices)
{
	m_pPrimitiveBatchTexture->Begin();// 描画開始
	m_pPrimitiveBatchTexture->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);// 四角形を描画
	m_pPrimitiveBatchTexture->End();// 描画終了

}
/*
*	@brief 板ポリゴン描画
*	@details 板ポリゴンを描画する(VertexPositionColorTexture用)
*	@param topology プリミティブトポロジー
*	@param vertices 頂点情報(VertexPositionColorTexture)
*	@param count 頂点数
*	@return なし
*/
void DrawPolygon::DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count)
{
	m_pPrimitiveBatchColorTexture->Begin();// 描画開始
	m_pPrimitiveBatchColorTexture->Draw(topology, vertices, count); // 頂点を描画
	m_pPrimitiveBatchColorTexture->End(); // 描画終了
}
/*
*	@brief 解放
*	@details 解放（頂点、テクスチャ）を行う
*	@param なし
*	@return なし
*/
void DrawPolygon::ReleasePositionTexture()
{
	m_pPrimitiveBatchTexture.reset();// プリミティブバッチ(頂点、テクスチャ)の解放
	m_pStates.reset(); // コモンステートの解放
	m_pDeviceContext = nullptr; // デバイスコンテキストの解放
	m_pDR = nullptr; // デバイスリソースの解放
	m_pDevice = nullptr; // デバイスの解放
}
/*
*	@brief 解放
*	@details 解放（頂点、色、テクスチャ）を行う
*	@param なし
*	@return なし
*/
void DrawPolygon::ReleasePositionColorTexture()
{
	m_pPrimitiveBatchColorTexture.reset();// プリミティブバッチ(頂点、色、テクスチャ)の解放
	m_pStates.reset(); // コモンステートの解放
	m_pDeviceContext = nullptr; // デバイスコンテキストの解放
	m_pDR = nullptr; // デバイスリソースの解放
	m_pDevice = nullptr; // デバイスの解放
}
/*
*	@brief シェーダーにバッファを送る
*	@details シェーダーにバッファを送る
*	@param startSlot シェーダーの開始スロット
*	@param numBuffers 送るバッファの数
*	@param ppBuffer 送るバッファの配列
*	@return なし
*/
void DrawPolygon::SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer)
{
	m_pDeviceContext->VSSetConstantBuffers(startSlot, numBuffers, ppBuffer);// 頂点シェーダーにバッファを送る
	m_pDeviceContext->GSSetConstantBuffers(startSlot, numBuffers, ppBuffer); // ジオメトリシェーダーにバッファを送る
	m_pDeviceContext->PSSetConstantBuffers(startSlot, numBuffers, ppBuffer); // ピクセルシェーダーにバッファを送る
}
/*
*	@brief シェーダーをセットする
*	@details シェーダーをセットする
*	@param shaders シェーダーの構造体
*	@param ppClassInstances クラスインスタンスの配列
*	@param nubClassInstances クラスインスタンスの数
*	@return なし
*/
void DrawPolygon::SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances)
{
	if (shaders.vs.Get() != nullptr) m_pDeviceContext->VSSetShader(shaders.vs.Get(), ppClassInstances, nubClassInstances);// 頂点シェーダーをセットする
	if (shaders.gs.Get() != nullptr) m_pDeviceContext->GSSetShader(shaders.gs.Get(), ppClassInstances, nubClassInstances); // ジオメトリシェーダーをセットする
	if (shaders.ps.Get() != nullptr) m_pDeviceContext->PSSetShader(shaders.ps.Get(), ppClassInstances, nubClassInstances); // ピクセルシェーダーをセットする
}
/*
*	@brief シェーダーを解放する
*	@details シェーダーを解放する
*	@param なし
*	@return なし
*/
void DrawPolygon::ReleaseShader()
{
	m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);// 頂点シェーダーを解放
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0); // ジオメトリシェーダーを解放
	m_pDeviceContext->PSSetShader(nullptr, nullptr, 0);// ピクセルシェーダーを解放
}
