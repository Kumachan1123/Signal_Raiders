/*
*	@file TextureManager.h
*	@brief テクスチャマネージャークラス
*/
#pragma once
// 標準ライブラリ
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>
// 前方宣言
class CommonResources;
class TextureManager
{
public:
	// アクセサ
	ID3D11ShaderResourceView* GetTexture(const std::string& key); // テクスチャの取得
public:// public関数
	TextureManager(); // コンストラクタ
	~TextureManager(); // デストラクタ
	void Initialize(ID3D11Device1* pDevice); // テクスチャの初期化
private:
	void LoadTexture(const std::string& key, const wchar_t* path); // テクスチャの読み込み
private:// 定数
	const std::string m_textureJsonPath = "Resources/Jsons/textures.json"; // テクスチャデータのJSONファイルパス
private:
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイス
	ID3D11Device1* m_pDevice;
	// テクスチャのマップ
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextureMap;

};