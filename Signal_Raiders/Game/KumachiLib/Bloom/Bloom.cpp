/*
*	@file Bloom.cpp
*	@brief ブルームエフェクトの実装ファイル
*	@details 関数を呼ぶだけでブルームエフェクトを実装できる
*/
#include <pch.h>
#include "Bloom.h"

// シングルトンインスタンスの初期化
std::unique_ptr<Bloom> Bloom::m_instance = nullptr;
/*
*	@brief シングルトンインスタンスを取得
*	@param なし
*	@return シングルトンインスタンス
*/
Bloom* const Bloom::GetInstance()
{
	if (m_instance == nullptr)// インスタンスがない場合
	{
		m_instance.reset(new Bloom());// インスタンスを生成
	}
	return m_instance.get();// インスタンスを返す
}
/*
*	@brief コンストラクタ
*	@param なし
*/
Bloom::Bloom()
	: m_context{}// デバイスコンテキスト
	, m_pDR{}// デバイスリソース
	, m_device{}// デバイス
	, m_pCommonResources{}// 共通リソース
	, m_blur1{}// ブラー1
	, m_blur2{}// ブラー2
	, m_basicPostProcess{}// ポストプロセス
	, m_dualPostProcess{}// デュアルポストプロセス
	, m_screenSize{}// スクリーンサイズ
	, m_states{}// コモンステート
{
}
/*
*	@brief デストラクタ
*	@param なし
*	@return なし
*/
Bloom::~Bloom()
{
	// 各種ポインターの解放
	m_pCommonResources = nullptr;// 共通リソース
	m_context = nullptr;// デバイスコンテキスト
	m_pDR = nullptr;// デバイスリソース
	m_device = nullptr;// デバイス
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
	m_device = m_pDR->GetD3DDevice();// デバイスを取得
	m_context = m_pDR->GetD3DDeviceContext();// デバイスコンテキストを取得
	this->CreateRenderTexture();// レンダーテクスチャを作成
	m_basicPostProcess = std::make_unique<BasicPostProcess>(m_device);// ポストプロセスを生成
	m_dualPostProcess = std::make_unique<DualPostProcess>(m_device);// デュアルポストプロセスを生成
}
/*
*	@brief オフスクリーン描画用にRTVを切り替える
*	@param なし
*	@return なし
*/
void Bloom::ChangeOffScreenRT()
{
	using namespace DirectX;
	m_offScreenRTV = m_offScreenRT->GetRenderTargetView();// オフスクリーン用のRTVを取得
	m_defaultDSV = m_pCommonResources->GetDeviceResources()->GetDepthStencilView();// デフォルトのDSVを取得
	m_context->OMSetRenderTargets(1, m_offScreenRTV.GetAddressOf(), m_defaultDSV.Get());// RTVとDSVをバインド
	m_context->ClearRenderTargetView(m_offScreenRTV.Get(), Colors::Black);// RTVをクリア
	m_context->ClearDepthStencilView(m_defaultDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);// DSVをクリア
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
	m_blur1.RTV = m_blur1RT->GetRenderTargetView();// ブラー1用のRTVを取得
	m_blur1.SRV = m_blur1RT->GetShaderResourceView();//	ブラー1用のSRVを取得
	m_blur2.RTV = m_blur2RT->GetRenderTargetView();// ブラー2用のRTVを取得
	m_blur2.SRV = m_blur2RT->GetShaderResourceView();//	ブラー2用のSRVを取得
	m_offScreenSRV = m_offScreenRT->GetShaderResourceView();// オフスクリーン用のSRVを取得
	ID3D11ShaderResourceView* nullsrv[] = { nullptr,nullptr };	// シェーダーリソースビューの解除
	D3D11_VIEWPORT vp = { 0.0f,0.0f,m_screenSize.right / 2.0f,m_screenSize.bottom / 2.0f,0.0f,1.0f };// ビューポートの設定
	m_context->RSSetViewports(1, &vp);// ビューポートを設定
	// -----------------------------------------------------
	//	Pass1::offscreen -> blur1 → 明るい部分を抽出する
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);// レンダーターゲットを設定
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);// エフェクトを設定
	m_basicPostProcess->SetBloomExtractParameter(0.35f);// 閾値を設定
	m_basicPostProcess->SetSourceTexture(m_offScreenSRV.Get());// オフスクリーン用のSRVを設定
	m_basicPostProcess->Process(m_context);// ポストプロセスを実行
	// -----------------------------------------------------
	//	Pass2::blur1 -> blur2 → 横にぼかす
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur2.RTV, nullptr);// レンダーターゲットを設定
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);// エフェクトを設定
	m_basicPostProcess->SetBloomBlurParameters(true, 1.250f, 1.025f);// 横ぼかしのパラメータを設定
	m_basicPostProcess->SetSourceTexture(m_blur1.SRV);// ブラー1用のSRVを設定
	m_basicPostProcess->Process(m_context);// ポストプロセスを実行
	m_context->PSSetShaderResources(0, 1, nullsrv);// シェーダーリソースビューを解除
	// -----------------------------------------------------
	//	Pass3::blur2 -> blur1 → 縦にぼかす
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);// レンダーターゲットを設定
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);// エフェクトを設定
	m_basicPostProcess->SetBloomBlurParameters(false, 1.250f, 1.025f);// 縦ぼかしのパラメータを設定
	m_basicPostProcess->SetSourceTexture(m_blur2.SRV);// ブラー2用のSRVを設定
	m_basicPostProcess->Process(m_context);// ポストプロセスを実行
	// -----------------------------------------------------
	// Pass4::offscreen -> blur1 → フレームバッファに描画する
	// -----------------------------------------------------
	m_defaultRTV = m_pCommonResources->GetDeviceResources()->GetRenderTargetView();// デフォルトのRTVを取得
	m_context->OMSetRenderTargets(1, m_defaultRTV.GetAddressOf(), nullptr);// レンダーターゲットを設定
	const auto& defaultVP = m_pCommonResources->GetDeviceResources()->GetScreenViewport();// デフォルトのビューポートを取得
	m_context->RSSetViewports(1, &defaultVP);// ビューポートを設定
	m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);// エフェクトを設定
	m_dualPostProcess->SetBloomCombineParameters(1.125f, 1.0f, 1.0f, 1.0f);// ブルームのパラメータを設定
	m_dualPostProcess->SetSourceTexture(m_offScreenSRV.Get());// オフスクリーン用のSRVを設定
	m_dualPostProcess->SetSourceTexture2(m_blur1.SRV); // ブラー1用のSRVを設定
	m_dualPostProcess->Process(m_context);// ポストプロセスを実行
	m_context->PSSetShaderResources(0, 2, nullsrv);// シェーダーリソースビューを解除
	m_context->OMSetRenderTargets(1, m_defaultRTV.GetAddressOf(), m_defaultDSV.Get());// レンダーターゲットを元に戻す
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
	m_offScreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// オフスクリーン用のRTを作成
	m_offScreenRT->SetDevice(m_device);// デバイスを設定
	m_offScreenRT->SetWindow(m_screenSize);// ウィンドウサイズを設定
	m_blur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// ブラー1用のRTを作成
	m_blur1RT->SetDevice(m_device);// デバイスを設定
	m_blur1RT->SetWindow(halfSize);// ウィンドウサイズを設定
	m_blur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// ブラー2用のRTを作成
	m_blur2RT->SetDevice(m_device);// デバイスを設定
	m_blur2RT->SetWindow(halfSize); // ウィンドウサイズを設定
}