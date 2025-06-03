/*
*  @file BossAppear.h
*  @brief ボス出現演出クラス
*/
#pragma once
// 標準ライブラリ
#include <Effects.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include <DeviceResources.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// クラスの前方宣言
class CommonResources;

class BossAppear
{
	// 構造体
private:
	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;		// 時間	
	};
public:// アクセサ
	// SE音量の設定
	void SetSEVolume(float volume) { m_seVolume = volume; }
public:// publicメンバ関数
	// コンストラクタ
	BossAppear();
	// デストラクタ
	~BossAppear();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// 終了処理
	void Finalize();
private:// privateメンバ関数
	// シェーダーの設定
	void SettingShader();
	// メインの演出を描画
	void DrawMain();
	// 背景の演出を描画
	void DrawBack();
private:// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// privateメンバ変数
	//	頂点シェーダ(Main)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	ピクセルシェーダ(Main)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	//  頂点シェーダ(Back)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShaderBack;
	// ピクセルシェーダ(Back)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderBack;
	// シェーダーの構造体(Main)
	DrawPolygon::Shaders m_shaders;
	// シェーダーの構造体(Back)
	DrawPolygon::Shaders m_shadersBack;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// コンスタントバッファ
	ConstBuffer m_constantBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// タイマー
	float m_timer;
	// シェーダーに渡すバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// SE音量
	float m_seVolume;
	// 再生フラグ
	bool m_isPlaying;
};