/*
*	@file ModelManager.cpp
*	@brief モデルマネージャークラス
*/
#include <pch.h>
#include "ModelManager.h"
// 外部ライブラリ
#include "Libraries/nlohmann/json.hpp"

/*
*	@brief コンストラクタ
*	@details モデルマネージャークラスのコンストラクタ
*	@param なし
*	@return なし
*/
ModelManager::ModelManager()
	: m_pDevice(nullptr) // デバイス
	, m_pModelMap() // モデルのマップ
	, m_skyModelPathMap() // 天球モデルパスのマップ
	, m_pEffectFactory(nullptr) // エフェクトファクトリー
{
}
/*
*	@brief デストラクタ
*	@details モデルマネージャークラスのデストラクタ
*	@param なし
*	@return なし
*/
ModelManager::~ModelManager()
{
	m_pModelMap.clear(); // モデルのマップをクリア
	m_pEffectFactory.reset(); // エフェクトファクトリーをリセット
	m_pDevice = nullptr; // デバイスをnullptrに設定
}
/*
*	@brief モデルの初期化
*	@details 各種モデルのロードとエフェクトの設定を行う
*	@param なし
*	@return なし
*/
void ModelManager::Initialize(ID3D11Device1* pDevice)
{
	m_pDevice = pDevice; // デバイスを設定
	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pDevice);// エフェクトファクトリーの作成
	m_pEffectFactory->SetSharing(false);// エフェクトの共有を無効にする
	LoadJsonFile();// JSONファイルの読み込み
	LoadSkyModelsJson(); // 天球モデルのパスをJSONファイルから読み込む
	SetupBulletModelEffects(); // 弾モデルの作成
	SetupStageModelEffects(); // ステージモデルの作成
}
/*
*	@brief JSONファイルの読み込み
*	@details モデルのパスをJSONファイルから読み込む
*	@param なし
*	@return なし
*/
void ModelManager::LoadJsonFile()
{
	using json = nlohmann::json;// nlohmann::jsonのエイリアスを定義
	using namespace std;// 標準名前空間を使用
	string filename = "Resources/Jsons/Models.json";//読み込むファイルの名前を作成
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
		m_pEffectFactory->SetDirectory(L"Resources/Models");// モデルのディレクトリを指定
		m_pModelMap[key] = DirectX::Model::CreateFromCMO(m_pDevice, wpath.c_str(), *m_pEffectFactory);// モデルを読み込む
	}

}
void ModelManager::LoadSkyModelsJson()
{
	using json = nlohmann::json;// nlohmann::jsonのエイリアスを定義
	using namespace std;// 標準名前空間を使用
	string filename = "Resources/Jsons/SkyModels.json";//読み込むファイルの名前を作成
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
		m_skyModelPathMap[key] = wpath; // モデルパスをマップに追加
	}
}
/*
*	@brief 弾モデルのエフェクト設定
*	@details 自弾と敵弾のモデルにエフェクトを設定する
*	@param なし
*	@return なし
*/
void ModelManager::SetupBulletModelEffects()
{
	m_pModelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// 自弾モデルのエフェクトを設定する
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);// エフェクトをBasicEffectにキャスト
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);// 拡散色を設定
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan); // 自発光色を設定
		});
	m_pModelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// 敵弾モデルのエフェクトを設定する
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);// エフェクトをBasicEffectにキャスト
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, 0.2f, 0, 1)); // 拡散色を設定
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red); // 環境光の色を設定
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato); // 自発光色を設定
		});
}

/*
*	@brief ステージモデルのエフェクト設定
*	@details ステージモデルにエフェクトを設定する
*	@param なし
*	@return なし
*/
void ModelManager::SetupStageModelEffects()
{
	m_pModelMap["Stage"]->UpdateEffects([](DirectX::IEffect* effect)// ステージモデルのエフェクトを設定する
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (!basicEffect)return;// エフェクトがnullptrの場合は処理を終える
			basicEffect->SetLightEnabled(0, false);// ライトを無効にする
			basicEffect->SetLightEnabled(1, false);// ライトを無効にする
			basicEffect->SetLightEnabled(2, false);// ライトを無効にする
			basicEffect->SetEmissiveColor(DirectX::Colors::White);// モデルを自発光させる
		}
	);
}
/*
*	@brief モデルを取得する
*	@details 指定されたキーに対応するモデルを取得する
*	@param key モデルのキー
*	@return 指定されたキーに対応するモデルのポインタ。見つからない場合はnullptrを返す
*/
DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	auto it = m_pModelMap.find(key);// キーに対応するモデルを検索
	if (it != m_pModelMap.end())	return it->second.get();// 見つかった場合はモデルを返す
	return nullptr;// 見つからなかった場合はnullptrを返す
}
/*
*	@brief 指定されたステージの空のモデルを作成して返す
*	@details ステージIDに応じた空のモデルを取得する
*	@param stageID ステージID
*	@return 指定されたステージの空のモデルのポインタ。見つからない場合はnullptrを返す
*/
DirectX::Model* ModelManager::GetSkyModel(const std::string& stageID)
{
	auto it = m_skyModelPathMap.find(stageID); // ステージIDに対応する空のモデルのパスを検索
	if (it == m_skyModelPathMap.end())	return nullptr; // 見つからなかった場合はnullptrを返す
	m_pModelMap["Sky"] = DirectX::Model::CreateFromCMO(m_pDevice, it->second.c_str(), *m_pEffectFactory); // モデルを保持
	return m_pModelMap["Sky"].get(); // モデルを返す
}
