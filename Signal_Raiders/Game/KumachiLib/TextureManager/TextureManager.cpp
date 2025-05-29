/*
*	@file TextureManager.cpp
*	@brief テクスチャマネージャークラス
*/
#include <pch.h>
#include "TextureManager.h"
/*
*	@brief コンストラクタ
*	@details テクスチャマネージャークラスのコンストラクタ
*	@param なし
*	@return なし
*/
TextureManager::TextureManager()
	: m_pCommonResources(nullptr) // 共通リソース
	, m_pDevice(nullptr) // デバイス
	, m_pTextureMap() // テクスチャのマップ
{
}
/*
*	@brief デストラクタ
*	@details テクスチャマネージャークラスのデストラクタ
*	@param なし
*	@return なし
*/
TextureManager::~TextureManager()
{
}
/*
*	@brief 初期化
*	@details テクスチャマネージャークラスの初期化
*	@param CommonResources* commonResources	共通リソースへのポインタ
*	@return なし
*/
void TextureManager::Initialize(CommonResources* commonResources)
{
	m_pCommonResources = commonResources; // 共通リソースの設定
	m_pDevice = m_pCommonResources->GetDeviceResources()->GetD3DDevice(); // デバイスの取得
	Load(); // テクスチャの読み込み
}
/*
*	@brief 読み込み
*	@details Jsonファイルからテクスチャパスとキーを読み込む
*	@param なし
*	@return なし
*/
void TextureManager::Load()
{
	//using json = nlohmann::json;
	//using namespace std;
	//string filename = "Resources/Jsons/Textures.json";//読み込むファイルの名前を作成
	//ifstream ifs(filename.c_str());//ファイルを開く
	//if (ifs.good())// ファイルが正常に開けたら
	//{
	//	json j;							//jsonオブジェクト
	//	ifs >> j;						//ファイルから読み込む
	//	ifs.close();					//ファイルを閉じる
	//	for (auto it = j.begin(); it != j.end(); ++it)
	//	{
	//		string key = it.key();               // テクスチャのキー
	//		string path = it.value();            // ファイルパス
	//		wstring wpath(path.begin(), path.end()); // std::string → std::wstring に変換
	//		LoadTexture(key, wpath.c_str()); // テクスチャの読み込み
	//	}
	//}
}
/*
*	@brief テクスチャの読み込み
*	@details 指定されたキーとパスでテクスチャを読み込み、マップに追加する
*	@param key テクスチャのキー
*	@param path テクスチャのパス
*	@return なし
*/
void TextureManager::LoadTexture(const std::string& key, const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// テクスチャ格納用
	DirectX::CreateWICTextureFromFile(m_pDevice, path, nullptr, texture.ReleaseAndGetAddressOf());// テクスチャ読み込み
	m_pTextureMap[key] = texture;           // マップに追加
}

ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& key)
{
	return nullptr;
}

void TextureManager::ClearTextures()
{
	return;
}
