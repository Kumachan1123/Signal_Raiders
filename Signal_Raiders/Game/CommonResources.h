/*
	@file	CommonResources.h
	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"

// 前方宣言
namespace DX
{
	class StepTimer;
	class DeviceResources;
}
namespace mylib
{
	class DebugString;
	class InputManager;
}

class CommonResources
{
public:// アクセサ
	DX::StepTimer* GetStepTimer() const { return m_stepTimer; }// タイマーを取得する
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }// デバイスリソースを取得する
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }// コモンステートを取得する
	mylib::DebugString* GetDebugString() const { return m_debugString; }// デバッグ文字列を取得する
	mylib::InputManager* GetInputManager() const { return m_inputManager; }// 入力マネージャを取得する
	AudioManager* GetAudioManager() const { return m_audioManager; }// オーディオマネージャを取得する
public:// publicメソッド
	CommonResources();// コンストラクタ
	~CommonResources() = default;// デストラクタ defaultで定義
	void Initialize(// 初期化する
		DX::StepTimer* timer,// 受け渡しするタイマー
		DX::DeviceResources* dr,// 受け渡しするデバイスリソース
		DirectX::CommonStates* commonStates,// 受け渡しするコモンステート
		mylib::DebugString* debugString,// 受け渡しするデバッグ文字列
		mylib::InputManager* inputManager,// 受け渡しする入力マネージャ
		AudioManager* audioManager// 受け渡しするオーディオマネージャ
	);
private:// privateメンバ変数
	// 受け渡しするリソース一覧
	// タイマー
	DX::StepTimer* m_stepTimer;
	// デバイスリソース
	DX::DeviceResources* m_deviceResources;
	// コモンステート
	DirectX::CommonStates* m_commonStates;
	// デバッグ文字列
	mylib::DebugString* m_debugString;
	// 入力マネージャ
	mylib::InputManager* m_inputManager;
	// オーディオマネージャ
	AudioManager* m_audioManager;
};
