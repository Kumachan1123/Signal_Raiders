/*
*	@file	PlayerController.cpp
*	@brief	プレイヤーコントローラークラス
*/
#include <pch.h>
#include "PlayerController.h"
// 通常移動速度
const float PlayerController::MOVE_SPEED = 5.0f;
// ダッシュ移動速度
const float PlayerController::DASH_SPEED = 15.0f;
// ダッシュ時間
const float PlayerController::DASH_TIME = 10.0f;
// 重力
const float PlayerController::GRAVITY = 0.75f;
// ピッチの制限
const float PlayerController::PITCH_LIMIT = 80.0f;
// マウス感度
const float PlayerController::MOUSE_SENSITIVE = 0.0025f;
// マウスX軸感度
const float PlayerController::MOUSE_X_SENSITIVE = 10.0f;
// マウスY軸感度
const float PlayerController::MOUSE_Y_SENSITIVE = 100.0f;
/*
*	@brief コンストラクタ
*	@details プレイヤーコントローラークラスのコンストラクタ
*	@param pPlayer プレイヤーのポインター
*	@return なし
*/
PlayerController::PlayerController(Player* pPlayer)
	: m_pPlayer{ pPlayer }// プレイヤーのポインター
	, m_position{}// プレイヤーの位置
	, m_velocity{}// プレイヤーの移動ベクトル
	, m_yaw{}// カメラのX軸回転
	, m_pitch{}// カメラのY軸回転
	, m_pitchLimit{ PlayerController::PITCH_LIMIT }// ピッチの制限
	, m_dash{}// ダッシュ速度
	, m_pCommonResources{}// 共通リソース
	, m_hWnd{ nullptr }// ウィンドウハンドル
	, m_point{}// マウスの位置
	, m_lastMouseX{ 0.0f }// マウスのX座標
	, m_lastMouseY{ 0.0f }// マウスのY座標
	, m_sensitive{ pPlayer->GetMouseSensitive() + 1.0f }// マウス感度:Player->GetMouseSensitive()のままだと最小値が0.0fになるので + 1.0fする
	, m_rotate{}// プレイヤーの回転
	, m_dashTime{ PlayerController::DASH_TIME }// ダッシュ時間
{

}
/*
*	@brief デストラクタ
*	@details プレイヤーコントローラークラスのデストラクタ
*	@param なし
*	@return なし
*/
PlayerController::~PlayerController() {/* do nothing. */ }
/*
*	@brief プレイヤーの移動を止める
*	@details プレイヤーの移動を止める
*	@param なし
*	@return なし
*/
void PlayerController::MoveStop()
{
	using namespace DirectX::SimpleMath;
	// 移動ベクトルをゼロにする
	m_velocity = Vector3::Zero;
	// 重力をかける
	m_velocity.y -= PlayerController::GRAVITY;
	// yawを初期化
	m_yaw = 0.0f;
	// pitchを初期化
	m_pitch = 0.0f;
	// 通常速度
	m_dash = PlayerController::MOVE_SPEED;
}
/*
*	@brief プレイヤーコントローラーの初期化
*	@details プレイヤーコントローラーの初期化(コモンリソースを渡す)
*	@param resources コモンリソース
*	@return なし
*/
void PlayerController::Initialize(CommonResources* resources)
{
	// コモンリソースを保存
	m_pCommonResources = resources;
	// windowハンドルを取得
	HWND hWnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウハンドルを保存
	m_hWnd = hWnd;
	// ウィンドウが表示されているモニターを取得
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	// モニター情報を取得
	MONITORINFO mi = {};
	// モニター情報のサイズを設定
	mi.cbSize = sizeof(MONITORINFO);
	// モニター情報を取得
	if (GetMonitorInfo(hMonitor, &mi))
	{
		// モニター領域の中央
		// X座標の中央
		int centerX = (mi.rcMonitor.left + mi.rcMonitor.right) / 2;
		// Y座標の中央
		int centerY = (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
		// マウスの位置を中央に設定
		m_point = { centerX, centerY };
		// マウスの位置をウィンドウ座標に変換
		SetCursorPos(centerX, centerY);
		// クライアント座標で中央を取得
		POINT clientCenter = { centerX, centerY };
		// クライアント座標をウィンドウ座標に変換
		ClientToScreen(hWnd, &clientCenter);
		// マウスのX座標を保存
		m_lastMouseX = (float)clientCenter.x;
		// マウスのY座標を保存
		m_lastMouseY = (float)clientCenter.y;
	}
}
/*
*	@brief プレイヤーの移動を更新
*	@details WASDで移動、マウスで視点を変更する
*	@param elapsedTime 前回のフレームからの経過時間
*	@return なし
*/
void PlayerController::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();
	// 移動を止める
	MoveStop();
	// マウスの座標
	POINT point;
	// マウスの位置を取得
	GetCursorPos(&point);
	// ウィンドウ座標に変換
	ScreenToClient(m_hWnd, &point);
	// マウスのX座標を取得
	int mouseX = point.x;
	// マウスのY座標を取得
	int mouseY = point.y;
	// マウスの相対移動を計算
	// マウスのX座標の差分
	float deltaX = static_cast<float>(mouseX - m_lastMouseX);
	// マウスのY座標の差分
	float deltaY = static_cast<float>(mouseY - m_lastMouseY);
	// マウス感度を適用
	const float mouseSensitivity = PlayerController::MOUSE_SENSITIVE * m_sensitive;
	// マウスのX座標の差分を計算
	float x = deltaX * mouseSensitivity * PlayerController::MOUSE_X_SENSITIVE * elapsedTime;
	// マウスのY座標の差分を計算
	float y = deltaY * mouseSensitivity * PlayerController::MOUSE_Y_SENSITIVE * elapsedTime;
	// 左右回転（yaw）
	m_yaw -= x;
	// 上下回転（pitch）
	m_pitch -= y;
	// ピッチの制限をラジアンに変換
	float pitchLimit = XMConvertToRadians(m_pitchLimit);
	// 上下の回転制限
	m_pitch = Clamp(m_pitch, -pitchLimit, pitchLimit);
	// 前後移動
	// Wキーが押されているとき、前方に進む
	if (kbTracker->lastState.W)
	{
		// X軸方向に移動する
		m_velocity.x += m_pPlayer->GetPlayerDir().x;
		// Z軸方向に移動する
		m_velocity.z += m_pPlayer->GetPlayerDir().z;
	}
	// Sキーが押されているとき、後方に進む
	if (kbTracker->lastState.S)
	{
		// X軸方向に移動する
		m_velocity.x -= m_pPlayer->GetPlayerDir().x;
		// Z軸方向に移動する
		m_velocity.z -= m_pPlayer->GetPlayerDir().z;
	}
	// カメラの右方向ベクトルを計算する(右を基準とする）
	// 右方向ベクトル
	Vector3 rightDirection;
	// X軸方向に移動する
	rightDirection.x = m_pPlayer->GetPlayerDir().z;
	// Z軸方向に移動する
	rightDirection.z = -m_pPlayer->GetPlayerDir().x;
	// 左右移動
	// Aキーが押されているとき、左方向に移動する
	if (kbTracker->lastState.A)
	{
		// X軸方向に移動する
		m_velocity.x += rightDirection.x;
		// Z軸方向に移動する
		m_velocity.z += rightDirection.z;
	}
	// Dキーが押されているとき, 右方向に移動する
	if (kbTracker->lastState.D)
	{
		// X軸方向に移動する
		m_velocity.x -= rightDirection.x;
		// Z軸方向に移動する
		m_velocity.z -= rightDirection.z;
	}
	// 左shiftキーが押されているときダッシュ
	if (kbTracker->lastState.LeftShift)
	{
		// ダッシュ時間が残っているとき
		if (m_dashTime > 0.0f)
		{
			// ダッシュ速度
			m_dash = PlayerController::DASH_SPEED;
			// ダッシュ時間を減らす
			m_dashTime -= elapsedTime;
		}
	}
	else// 左shiftキーが押されていないとき
	{
		// 通常速度
		m_dash = PlayerController::MOVE_SPEED;
		// ダッシュ時間を増やす
		m_dashTime += elapsedTime / 2;
		// ダッシュ時間が上限を超えたときこれ以上増やさない
		if (m_dashTime >= PlayerController::DASH_TIME)m_dashTime = PlayerController::DASH_TIME;
	}
	// 移動量があるとき正規化する
	if (m_velocity.LengthSquared() > 0)	m_velocity.Normalize();
	// 移動量を補正する
	m_velocity *= m_dash * elapsedTime;
	// 実際に移動する
	m_position += m_velocity;
	// 横回転を保存
	m_rotate = x;
	// 地面の高さに制限
	if (m_position.y <= Stage::STAGE_HEIGHT)m_position.y = Stage::STAGE_HEIGHT;
	// 地面の面積に制限
	if (m_position.x <= -Stage::STAGE_SIZE) m_position.x = -Stage::STAGE_SIZE;// 左端
	if (m_position.x >= Stage::STAGE_SIZE) m_position.x = Stage::STAGE_SIZE;// 右端
	if (m_position.z <= -Stage::STAGE_SIZE) m_position.z = -Stage::STAGE_SIZE;// 上端
	if (m_position.z >= Stage::STAGE_SIZE) m_position.z = Stage::STAGE_SIZE;// 下端
	// マウス位置を中央に戻す
	SetCursorPos(m_point.x, m_point.y);
	// 次のフレーム用にlastMouseX, lastMouseYをクライアント中央にリセット
	POINT clientCenter = { m_point.x,m_point.y };
	// クライアント座標をウィンドウ座標に変換
	ScreenToClient(m_hWnd, &clientCenter);
	// マウスのX座標を保存
	m_lastMouseX = static_cast<float>(clientCenter.x);
	// マウスのY座標を保存
	m_lastMouseY = static_cast<float>(clientCenter.y);
}
/*
*	@brief デバッグコマンド
*	@details デバッグ時のみ使用可能なチートコマンド
*	@param なし
*	@return なし
*/
void PlayerController::DebugCommand()
{
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();
	// マウスの状態を取得する
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 右クリックで敵を一掃
	if (MyMouse::IsRightMouseButtonPressed(mouseState) && m_pPlayer->GetisCheat() == false)
	{
		// チートフラグを立てる
		m_pPlayer->SetisCheat(true);
		// 敵のHPを1000減らす（即死）
		for (auto& enemy : m_pPlayer->GetEnemies()->GetEnemies())enemy->ApplyDamageToEnemy(1000);
	}
	// 右クリックされてないときチートコマンドを無効にする
	if (!MyMouse::IsRightMouseButtonPressed(mouseState))m_pPlayer->SetisCheat(false);
	// スペースキーでプレイヤーのHPを0にする
	if (kbTracker->pressed.Space)m_pPlayer->SetPlayerHP(0.0f);
}
/*
*	@brief プレイヤーが弾を撃つ
*	@details 左クリックで弾を発射
*	@param なし
*	@return なし
*/
void PlayerController::Shoot()
{
	// マウスの状態を取得する
	const auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	if (MyMouse::IsLeftMouseButtonPressed(mouseState) &&// 左クリックされていて
		m_pPlayer->GetBulletManager()->GetIsPlayerShoot() == false &&// 弾が発射されていなくて
		m_pPlayer->GetBulletManager()->GetPlayerBulletCount() > 0 && // 弾が残っていて
		m_pPlayer->GetBulletManager()->GetIsReloading() == false)// リロード中でないとき
	{
		// 弾を生成する
		m_pPlayer->CreateBullet();
		// 弾を消費する
		m_pPlayer->GetBulletManager()->ConsumePlayerBullet();
	}
	// 左クリックされていないとき弾を撃ったフラグを下ろす
	if (!MyMouse::IsLeftMouseButtonPressed(mouseState))m_pPlayer->GetBulletManager()->SetIsPlayerShoot(false);
}
/*
*	@brief プレイヤーの弾をリロードする
*	@details Rキーで弾をリロードする
*	@param なし
*	@return なし
*/
void PlayerController::Reload()
{
	// キーボードステートトラッカーを取得する
	const auto& kbState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// Rキーでリロードフラグを立てる
	if (kbState.R)m_pPlayer->GetBulletManager()->SetIsReloading(true);
	// リロード中なら弾を補充する
	if (m_pPlayer->GetBulletManager()->GetIsReloading())m_pPlayer->GetBulletManager()->ReLoadPlayerBullet();
}
