/*
*	@file Bloom.h
*	@brief ブルームエフェクトのヘッダファイル
*	@details 関数を呼ぶだけでブルームエフェクトを実装できる
*/
#pragma once
// DirectXのヘッダファイル
#include <PostProcess.h>
#include <CommonStates.h>
// 外部ライブラリ
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Libraries/Microsoft/RenderTexture/RenderTexture.h"
//前方宣言
class CommonResources;

class Bloom
{
public:
	// シングルトンインスタンスを取得
	static Bloom* const GetInstance();
public:
	// 構造体
	struct Blur// ブラー
	{
		ID3D11RenderTargetView* RTV;// レンダーターゲットビュー
		ID3D11ShaderResourceView* SRV;// シェーダーリソースビュー
	};

public:
	// public関数
	~Bloom();// デストラクタ
	void CreatePostProcess(CommonResources* resources);// ポストプロセスを生成
	void ChangeOffScreenRT();// オフスクリーン描画用にRTVを切り替える
	void PostProcess();// ポストプロセスに必要な設定を準備する
private:
	// private関数
	Bloom();// コンストラクタ
	void CreateRenderTexture();// レンダーテクスチャを作成する
	Bloom(const Bloom&) = delete;// コピーコンストラクタ
	Bloom& operator=(const Bloom&) = delete;// 代入演算子の禁止
private:
	//private変数
	// シングルトンインスタンス
	static std::unique_ptr<Bloom> m_instance;
	// 共通リソース
	CommonResources* m_commonResources;
	// 共通リソース
	ID3D11DeviceContext1* m_context;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// デバイス
	ID3D11Device1* m_device;
	// ブルームエフェクトのコモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ブラー
	Blur m_blur1;
	Blur m_blur2;
	// ポストプロセス
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;
	// デュアルポストプロセス
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;
	// スクリーンサイズ
	RECT m_screenSize;
	// レンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_offScreenRT;// オフスクリーン用のRT
	std::unique_ptr<DX::RenderTexture> m_blur1RT;// ブラー1用のRT
	std::unique_ptr<DX::RenderTexture> m_blur2RT;// ブラー2用のRT
	std::unique_ptr<DX::RenderTexture> m_savedOffScreenRT;// 保存されたオフスクリーン用のRT
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

