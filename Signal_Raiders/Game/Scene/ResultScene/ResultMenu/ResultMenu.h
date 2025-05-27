/*
	@file	ResultMenu.h
	@brief	リザルトメニュークラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class ResultMenu : public IMenuUI
{
public:// 列挙型
	enum SceneID// シーンID
	{
		REPLAY = 0,// リプレイ
		SELECT_STAGE,// ステージ選択
	};
public:// アクセサ
	SceneID GetSceneNum() const { return m_num; }// シーン番号取得
	void SetSceneNum(SceneID num) { m_num = num; }// シーン番号設定
	bool GetIsHit()const { return m_hit; }// UIにヒットしたかどうか取得
	void SetIsHit(bool hit) { m_hit = hit; }// UIにヒットしたかどうか設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SEの音量設定
public:// public関数
	ResultMenu();// コンストラクタ
	~ResultMenu();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// 更新(contextから時間を取得して更新)
	void Render()override;// 描画
	void Add(const wchar_t* path// メニューアイテムを追加
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type)override;
private:// private関数
	void Update(float elapsedTime);// 更新
private:// private定数
	static const int INVALID_MENU_INDEX;// 無効なメニューインデックス
private:// private変数
	// メニューアイテムの数
	unsigned int m_menuIndex;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 選択可能なUI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// 選択不可能なUI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// 選択可能なUIが選ばれているときに出るUI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// テクスチャパス
	const wchar_t* m_pSelectTexturePath;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// シーンID
	SceneID m_num;
	// 時間
	float m_time;
	// UIのヒットフラグ
	bool m_hit;
	// SEの音量
	float m_SEVolume;
	// 再生フラグ
	bool m_isSEPlay;
};