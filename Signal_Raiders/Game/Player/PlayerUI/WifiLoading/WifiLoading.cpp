/*
*	@file WifiLoading.cpp
*	@brief Wi-Fiローディングクラス
*/
#include <pch.h>
#include "WifiLoading.h"
/*
*	@brief コンストラクタ
*	@details ウィンドウの幅と高さを初期化
*	@param なし
*	@return なし
*/
WifiLoading::WifiLoading()
	: m_windowHeight(0)			// ウィンドウの高さ
	, m_windowWidth(0)			// ウィンドウの幅
	, m_pDR(nullptr)			// デバイスリソース
	, m_pLoading(nullptr)		// ローディングUI
	, m_pLoadgingText(nullptr)	// ローディングテキスト
	, m_animSpeed(1)			// アニメーションスピード
	, m_anim(0)					// アニメーションカウント
	, m_animTime(0)				// アニメーション時間
	, m_frameRows(1)			// 画像の行数
	, m_frameCols(4)			// 画像の列数
{
}
/*
*	@brief デストラクタ
*	@details ここでは何もしない
*	@param なし
*	@return なし
*/
WifiLoading::~WifiLoading() { /*do nothing*/ }
/*
*	@brief 初期化関数
*	@details デバイスリソースを取得し、ウィンドウの幅と高さを設定後、ローディングUIを作成する
*	@param resources デバイスリソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void WifiLoading::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();// デバイスリソースを取得
	m_windowWidth = width;// ウィンドウの幅
	m_windowHeight = height;// ウィンドウの高さ
	// ローディング画像
	Add(m_pLoading, L"Resources/Textures/Loading.png",
		DirectX::SimpleMath::Vector2((float)(m_windowWidth), (float)(m_windowHeight)),
		DirectX::SimpleMath::Vector2(.4f, .4f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ローディングテキスト
	Add(m_pLoadgingText, L"Resources/Textures/Loading_Text.png",
		DirectX::SimpleMath::Vector2((float)(m_windowWidth - 70), (float)(m_windowHeight)),
		DirectX::SimpleMath::Vector2(.4f, .4f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ローディング画像のアニメーション設定
	m_pLoading->SetFrameRows(m_frameRows);// フレームの行数を設定
	m_pLoading->SetFrameCols(m_frameCols);// フレームの列数を設定
}
/*
*	@brief 更新関数(内部用)
*	@details ローディングの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void WifiLoading::Update(float elapsedTime)
{
	m_animTime += elapsedTime * m_animSpeed;// 経過時間を加算
	if (m_animTime > 1.25f)m_animTime = 0.0f;// 一定期間を超えたらタイマーをリセット
	float stepRatio = floor(m_animTime * m_frameCols) / m_frameCols;// 0.25 刻みに調整
	m_pLoading->SetRenderRatio(stepRatio);// アニメーションの比率を設定
}
/*
*	@brief 描画関数
*	@details ローディングの描画を行う
*	@param なし
*	@return なし
*/
void WifiLoading::Render()
{
	m_pLoading->SetShaderType(PlayerUI::ShaderType::ANIM);// シェーダータイプをアニメーションに設定
	m_pLoading->Render();// ローディング画像を描画
	m_pLoadgingText->SetShaderType(PlayerUI::ShaderType::OTHER);// シェーダータイプをその他に設定
	m_pLoadgingText->Render();// ローディングテキストを描画
}
/*
*	@brief UIの追加関数
*	@details 指定されたパスのUIを追加する
*	@param pPlayerUI 追加するUIのスマートポインタ
*	@param path UIのテクスチャパス
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void WifiLoading::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();// PlayerUIのインスタンスを生成
	// UIの初期化
	pPlayerUI->Create(m_pDR	// デバイスリソース
		, path				// テクスチャのパス
		, position			// UIの位置
		, scale				// UIのスケール
		, anchor);			// UIのアンカーを設定
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定
}
