/*
*	@file TextureManager.h
*	@brief テクスチャマネージャークラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>
 // 外部ライブラリ
#include "nlohmann/json.hpp"
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class TextureManager
{
public:
	TextureManager();	// コンストラクタ
	~TextureManager();	// デストラクタ
	void Initialize(CommonResources* commonResources); // 初期化
	void Load();// Jsonファイルからテクスチャパスとキーを読み込む
	void LoadTexture(const std::string& key, const wchar_t* path); // テクスチャの読み込み
	ID3D11ShaderResourceView* GetTexture(const std::string& key); // テクスチャの取得
	void ClearTextures(); // テクスチャのクリア

private:
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイス
	ID3D11Device1* m_pDevice;
	// テクスチャのマップ
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextureMap;
};