// ブルームエフェクトのヘッダファイル

#pragma once
#include <PostProcess.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "Game/KumachiLib/RenderTexture/RenderTexture.h"
//前方宣言
class CommonResources;
class Bloom
{
public:
	// シングルトンインスタンスを取得
	static Bloom* const GetInstance();
public:
	// 構造体
	struct Blur
	{
		ID3D11RenderTargetView* RTV;
		ID3D11ShaderResourceView* SRV;
	};

public:
	// デストラクタ
	~Bloom();

	// ポストプロセスを生成
	void CreatePostProcess(CommonResources* resources);
	// オフスクリーン描画用にRTVを切り替える
	void ChangeOffScreenRT();
	// ポストプロセスに必要な設定を準備する
	void PostProcess();
	// レンダーターゲットを元に戻す
	void ChangeDefaultRT();
private:
	// コンストラクタ
	Bloom();
	// レンダーテクスチャを作成する
	void CreateRenderTexture();
	// コピーコンストラクタと代入演算子の禁止
	Bloom(const Bloom&) = delete;
	Bloom& operator=(const Bloom&) = delete;;

private:
	// シングルトンインスタンス
	static std::unique_ptr<Bloom> m_instance;
	// 共通リソース
	CommonResources* m_commonResources;
	// 共通リソース
	ID3D11DeviceContext1* m_context;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	ID3D11Device1* m_device;
	// ブルームエフェクトのコモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ブラー
	Blur m_blur1;
	Blur m_blur2;
	// ポストプロセス
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;
	// スクリーンサイズ
	RECT m_screenSize;
	// レンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_offScreenRT;
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;
	std::unique_ptr<DX::RenderTexture> m_savedOffScreenRT;
	// オフスクリーン用のレンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_offScreenRTV;
	// オフスクリーン用のシェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_offScreenSRV;
	// デフォルトの深度ステンシルビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_defaultDSV;
	// デフォルトのレンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_defaultRTV;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

};

