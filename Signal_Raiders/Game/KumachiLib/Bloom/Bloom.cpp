/*
*	@file Bloom.cpp
*	@brief ブルームエフェクトの実装ファイル
*	@details 関数を呼ぶだけでブルームエフェクトを実装できる
*/
#include <pch.h>
#include "Bloom.h"

// シングルトンインスタンスの初期化
std::unique_ptr<Bloom> Bloom::m_pInstance = nullptr;
/*
*	@brief シングルトンインスタンスを取得
*	@details Bloomクラスのシングルトンインスタンスを取得する
*	@param なし
*	@return シングルトンインスタンス
*/
Bloom* const Bloom::GetInstance()
{
	if (m_pInstance == nullptr)// インスタンスがない場合
	{
		m_pInstance.reset(new Bloom());// インスタンスを生成
	}
	return m_pInstance.get();// インスタンスを返す
}
/*
*	@brief コンストラクタ
*	@details ブルームエフェクトの初期化を行う
*	@param なし
*	@return なし
*/
Bloom::Bloom()
	: m_pDeviceContext{}// デバイスコンテキスト
	, m_pDR{}// デバイスリソース
	, m_pDevice{}// デバイス
	, m_pCommonResources{}// 共通リソース
	, m_blur1{}// ブラー1
	, m_blur2{}// ブラー2
	, m_pBasicPostProcess{}// ポストプロセス
	, m_pDualPostProcess{}// デュアルポストプロセス
	, m_screenSize{}// スクリーンサイズ
	, m_pStates{}// コモンステート
{
}
/*
*	@brief デストラクタ
*	@details ブルームエフェクトの終了処理を行う
*	@param なし
*	@return なし
*/
Bloom::~Bloom()
{
	// 各種ポインターの解放
	m_pCommonResources = nullptr;// 共通リソース
	m_pDeviceContext = nullptr;// デバイスコンテキスト
	m_pDR = nullptr;// デバイスリソース
	m_pDevice = nullptr;// デバイス
}
/*
*	@brief ポストプロセスを生成
*	@details ブルームエフェクトを実装するためのポストプロセスを生成する
*	@param resources 共通リソース
*	@return なし
*/
void Bloom::CreatePostProcess(CommonResources* resources)
{
	using namespace DirectX;
	m_pCommonResources = resources;// 共通リソースを取得
	m_pDR = m_pCommonResources->GetDeviceResources();// デバイスリソースを取得
	m_pDevice = m_pDR->GetD3DDevice();// デバイスを取得
	m_pDeviceContext = m_pDR->GetD3DDeviceContext();// デバイスコンテキストを取得
	this->CreateRenderTexture();// レンダーテクスチャを作成
	m_pBasicPostProcess = std::make_unique<BasicPostProcess>(m_pDevice);// ポストプロセスを生成
	m_pDualPostProcess = std::make_unique<DualPostProcess>(m_pDevice);// デュアルポストプロセスを生成
}
/*
*	@brief オフスクリーン描画用にRTVを切り替える
*	@details ブルームエフェクトを実装するために、オフスクリーン描画用のレンダーテクスチャを設定する
*	@param なし
*	@return なし
*/
void Bloom::ChangeOffScreenRT()
{
	using namespace DirectX;
	m_pOffScreenRTV = m_pOffScreenRT->GetRenderTargetView();// オフスクリーン用のRTVを取得
	m_pDefaultDSV = m_pCommonResources->GetDeviceResources()->GetDepthStencilView();// デフォルトのDSVを取得
	m_pDeviceContext->OMSetRenderTargets(1, m_pOffScreenRTV.GetAddressOf(), m_pDefaultDSV.Get());// RTVとDSVをバインド
	m_pDeviceContext->ClearRenderTargetView(m_pOffScreenRTV.Get(), Colors::Black);// RTVをクリア
	m_pDeviceContext->ClearDepthStencilView(m_pDefaultDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);// DSVをクリア
}
/*
*	@brief ポストプロセスに必要な設定を準備する
*	@details ブルームエフェクトを実装するためのポストプロセスを実行する
*	@param なし
*	@return なし
*/
void Bloom::PostProcess()
{
	using namespace DirectX;
	m_blur1.RTV = m_pBlur1RT->GetRenderTargetView();// ブラー1用のRTVを取得
	m_blur1.SRV = m_pBlur1RT->GetShaderResourceView();//	ブラー1用のSRVを取得
	m_blur2.RTV = m_pBlur2RT->GetRenderTargetView();// ブラー2用のRTVを取得
	m_blur2.SRV = m_pBlur2RT->GetShaderResourceView();//	ブラー2用のSRVを取得
	m_pOffScreenSRV = m_pOffScreenRT->GetShaderResourceView();// オフスクリーン用のSRVを取得
	ID3D11ShaderResourceView* nullsrv[] = { nullptr,nullptr };	// シェーダーリソースビューの解除
	D3D11_VIEWPORT vp = { 0.0f,0.0f,m_screenSize.right / 2.0f,m_screenSize.bottom / 2.0f,0.0f,1.0f };// ビューポートの設定
	m_pDeviceContext->RSSetViewports(1, &vp);// ビューポートを設定
	// -----------------------------------------------------
	//	Pass1::offscreen -> blur1 → 明るい部分を抽出する
	// -----------------------------------------------------
	m_pDeviceContext->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);// レンダーターゲットを設定
	m_pBasicPostProcess->SetEffect(BasicPostProcess::BloomExtract);// エフェクトを設定
	m_pBasicPostProcess->SetBloomExtractParameter(0.35f);// 閾値を設定
	m_pBasicPostProcess->SetSourceTexture(m_pOffScreenSRV.Get());// オフスクリーン用のSRVを設定
	m_pBasicPostProcess->Process(m_pDeviceContext);// ポストプロセスを実行
	// -----------------------------------------------------
	//	Pass2::blur1 -> blur2 → 横にぼかす
	// -----------------------------------------------------
	m_pDeviceContext->OMSetRenderTargets(1, &m_blur2.RTV, nullptr);// レンダーターゲットを設定
	m_pBasicPostProcess->SetEffect(BasicPostProcess::BloomBlur);// エフェクトを設定
	m_pBasicPostProcess->SetBloomBlurParameters(true, 1.250f, 1.025f);// 横ぼかしのパラメータを設定
	m_pBasicPostProcess->SetSourceTexture(m_blur1.SRV);// ブラー1用のSRVを設定
	m_pBasicPostProcess->Process(m_pDeviceContext);// ポストプロセスを実行
	m_pDeviceContext->PSSetShaderResources(0, 1, nullsrv);// シェーダーリソースビューを解除
	// -----------------------------------------------------
	//	Pass3::blur2 -> blur1 → 縦にぼかす
	// -----------------------------------------------------
	m_pDeviceContext->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);// レンダーターゲットを設定
	m_pBasicPostProcess->SetEffect(BasicPostProcess::BloomBlur);// エフェクトを設定
	m_pBasicPostProcess->SetBloomBlurParameters(false, 1.250f, 1.025f);// 縦ぼかしのパラメータを設定
	m_pBasicPostProcess->SetSourceTexture(m_blur2.SRV);// ブラー2用のSRVを設定
	m_pBasicPostProcess->Process(m_pDeviceContext);// ポストプロセスを実行
	// -----------------------------------------------------
	// Pass4::offscreen -> blur1 → フレームバッファに描画する
	// -----------------------------------------------------
	m_pDefaultRTV = m_pCommonResources->GetDeviceResources()->GetRenderTargetView();// デフォルトのRTVを取得
	m_pDeviceContext->OMSetRenderTargets(1, m_pDefaultRTV.GetAddressOf(), nullptr);// レンダーターゲットを設定
	const auto& defaultVP = m_pCommonResources->GetDeviceResources()->GetScreenViewport();// デフォルトのビューポートを取得
	m_pDeviceContext->RSSetViewports(1, &defaultVP);// ビューポートを設定
	m_pDualPostProcess->SetEffect(DualPostProcess::BloomCombine);// エフェクトを設定
	m_pDualPostProcess->SetBloomCombineParameters(1.125f, 1.0f, 1.0f, 1.0f);// ブルームのパラメータを設定
	m_pDualPostProcess->SetSourceTexture(m_pOffScreenSRV.Get());// オフスクリーン用のSRVを設定
	m_pDualPostProcess->SetSourceTexture2(m_blur1.SRV); // ブラー1用のSRVを設定
	m_pDualPostProcess->Process(m_pDeviceContext);// ポストプロセスを実行
	m_pDeviceContext->PSSetShaderResources(0, 2, nullsrv);// シェーダーリソースビューを解除
	m_pDeviceContext->OMSetRenderTargets(1, m_pDefaultRTV.GetAddressOf(), m_pDefaultDSV.Get());// レンダーターゲットを元に戻す
}
/*
*	@brief レンダーテクスチャを作成する
*	@details ブルームエフェクトを実装するためのレンダーテクスチャを作成する
*	@param なし
*	@return なし
*/
void Bloom::CreateRenderTexture()
{
	m_screenSize = m_pCommonResources->GetDeviceResources()->GetOutputSize();// スクリーンサイズを取得
	RECT halfSize{ 0,0, m_screenSize.right / 2, m_screenSize.bottom / 2 };// 半分のサイズを取得
	m_pOffScreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// オフスクリーン用のRTを作成
	m_pOffScreenRT->SetDevice(m_pDevice);// デバイスを設定
	m_pOffScreenRT->SetWindow(m_screenSize);// ウィンドウサイズを設定
	m_pBlur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// ブラー1用のRTを作成
	m_pBlur1RT->SetDevice(m_pDevice);// デバイスを設定
	m_pBlur1RT->SetWindow(halfSize);// ウィンドウサイズを設定
	m_pBlur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// ブラー2用のRTを作成
	m_pBlur2RT->SetDevice(m_pDevice);// デバイスを設定
	m_pBlur2RT->SetWindow(halfSize); // ウィンドウサイズを設定
}