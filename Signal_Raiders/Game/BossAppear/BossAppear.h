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
#include "DeviceResources.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// クラスの前方宣言
class CommonResources;

class BossAppear
{
private:
	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;    // 時間	
	};
public:
	BossAppear();
	~BossAppear();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	void SettingShader();
	void LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture);
	void DrawMain();
	void DrawBack();
	//	頂点シェーダ(Main)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ(Main)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//  頂点シェーダ(Back)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShaderBack;
	// ピクセルシェーダ(Back)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderBack;

	// シェーダーの構造体(Main)
	DrawPolygon::Shaders m_shaders;
	// シェーダーの構造体(Back)
	DrawPolygon::Shaders m_shadersBack;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	DirectX::SimpleMath::Matrix m_world;// ワールド行列
	DirectX::SimpleMath::Matrix m_view;// ビュー行列
	DirectX::SimpleMath::Matrix m_proj;	// プロジェクション行列
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// コンスタントバッファ
	ConstBuffer m_constantBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 共通リソース
	CommonResources* m_commonResources;
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//	変数
	DX::DeviceResources* m_pDR;
	//	タイマー
	float m_timer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	DirectX::VertexPositionTexture m_vertices;
};