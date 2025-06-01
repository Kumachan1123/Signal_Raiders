/*
*	@file TextureManager.cpp
*	@brief テクスチャマネージャークラスの実装
*/
#include <pch.h>
#include "TextureManager.h"
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief コンストラクタ
*	@details テクスチャマネージャークラスのコンストラクタ
*	@param なし
*	@return なし
*/
TextureManager::TextureManager()
	: m_pCommonResources(nullptr)// 共通リソース
	, m_pDevice(nullptr)// デバイス
	, m_pTextureMap()// テクスチャマップ
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
	for (auto& pair : m_pTextureMap)// テクスチャマップの解放
		pair.second.Reset(); // ComPtrのリセット
	m_pTextureMap.clear(); // マップのクリア
}
/*
*	@brief テクスチャの初期化
*	@details テクスチャマネージャークラスの初期化を行う
*	@param pDevice デバイス
*	@return なし
*/
void TextureManager::Initialize(ID3D11Device1* pDevice)
{
	using json = nlohmann::json;// nlohmann::jsonのエイリアスを定義
	using namespace std;// 標準名前空間を使用
	m_pDevice = pDevice; // デバイスの設定
	string filename = "Resources/Jsons/Textures.json";//読み込むファイルの名前を作成
	ifstream ifs(filename.c_str());//ファイルを開く
	if (!ifs.good())return;// ファイルが正常に開けなかったら強制終了
	json j;							//jsonオブジェクト
	ifs >> j;						//ファイルから読み込む
	ifs.close();					//ファイルを閉じる
	for (const auto& item : j.items())// JSONの各アイテムに対してループ
	{
		std::string key = item.key();                    // キー
		std::string path = item.value();                 // 値（ファイルパス）
		std::wstring wpath(path.begin(), path.end());    // 文字列変換
		LoadTexture(key, wpath.c_str());                 // 読み込み関数
	}
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
/*
*	@brief テクスチャの取得
*	@details 指定されたキーのテクスチャを取得する
*	@param key テクスチャのキー
*	@return 指定されたキーのテクスチャへのポインタ
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& key) { return m_pTextureMap[key].Get(); }// 指定されたキーのテクスチャを取得