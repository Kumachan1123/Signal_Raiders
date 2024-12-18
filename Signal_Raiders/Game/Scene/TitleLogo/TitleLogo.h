/*
	@file	TitleLogo.h
	@brief	タイトルロゴクラス
*/
#pragma once
#include <DeviceResources.h>
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
#include <cassert>
#include "Libraries/MyLib/InputManager.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "DeviceResources.h"
// 前方宣言
class CommonResources;

class TitleLogo final
{
public:
	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	ワールド行列
		DirectX::SimpleMath::Matrix		matView;	//	ビュー行列
		DirectX::SimpleMath::Matrix		matProj;	//	プロジェクション行列
		DirectX::SimpleMath::Vector4	Colors;
		DirectX::SimpleMath::Vector4 time;	//	時間
	};

private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成
	CreateShader* m_pCreateShader;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 変数
	DX::DeviceResources* m_pDR;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// データ受け渡し用コンスタントバッファ
	ConstBuffer m_ConstBuffer;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// タイトル画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_titleTexture;
	// 時間
	float m_time;
	// 空の行列
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	TitleLogo(CommonResources* resources);
	~TitleLogo();
	void LoadTexture(const wchar_t* path);

	//	初期化
	void Create(DX::DeviceResources* pDR);

	//	更新
	void Update(float elapsedTime);
	//	描画
	void Render();
private:
	//	シェーダーを作成
	void CreateShader();
};