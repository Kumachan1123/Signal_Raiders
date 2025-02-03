/**
 * @file   UpdateInfo.h
 *
 * @brief  Wi-Fiの情報を更新するソースファイル
 *
 * @author 制作者名：くまち
 *
 * @date   2024/04/22
 */
 // ヘッダファイルの読み込み ===================================================
#include "Game/Screen.h"
#include <sstream>
#include "pch.h"
#include "UpdateInfo.h"
#include "Game/Wifi/Wifi.h"
// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param[in] なし
 */
UpdateInfo::UpdateInfo(Wifi* pWifi)
	: m_InterfacesList{ nullptr }
	, m_Checkversion{ nullptr }
	, m_StartScan{ nullptr }
	, m_GetResults{ nullptr }
	, m_handle{ nullptr }
	, m_Preparation{ nullptr }
	, m_ProcessingScanResults{ nullptr }
	, m_pWifi{ pWifi }
{
}
/**
 * @brief デストラクタ
 */
UpdateInfo::~UpdateInfo()
{
}
void UpdateInfo::Initialize()
{
	m_InterfacesList = std::make_unique<GetListOfInterfaces>();
	m_Checkversion = std::make_unique<CheckVersion>();
	m_StartScan = std::make_unique<StartScan>();
	m_GetResults = std::make_unique<GetScanResults>();
	m_Preparation = std::make_unique<Preparation>();
	m_ProcessingScanResults = std::make_unique< ProcessingScanResults>();
}


void UpdateInfo::Set()
{
	// Wi-Fiハンドルの初期化
	m_handle->Initialize(m_pWifi);
	// バージョンの確認
	m_Checkversion->Check(m_pWifi);
	// インターフェースのリスト取得
	m_InterfacesList->GetList(m_pWifi);
	// スキャンの開始
	m_StartScan->Scan(m_pWifi);
	// スキャン結果の取得
	m_GetResults->GetResults(m_pWifi);
	if (m_pWifi->GetResult() != ERROR_SUCCESS)
	{
		return;
	}
	// 表示準備
	m_Preparation->SetUp(m_pWifi);
	// スキャン結果の処理
	m_ProcessingScanResults->GetResults(m_pWifi);
}

