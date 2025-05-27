/*
*	@file	PlayerController.cpp
*	@brief	プレイヤーコントローラークラス
*/
#include "pch.h"
#include "PlayerController.h"
const float PlayerController::MOVE_SPEED = 5.0f;// 通常移動速度
const float PlayerController::DASH_SPEED = 15.0f;// ダッシュ移動速度
const float PlayerController::DASH_TIME = 10.0f;// ダッシュ時間
const float PlayerController::GRAVITY = 0.75f;// 重力
const float PlayerController::PITCH_LIMIT = 80.0f;// ピッチの制限
const float PlayerController::MOUSE_SENSITIVE = 0.0025f;// マウス感度
const float PlayerController::MOUSE_X_SENSITIVE = 10.0f;// マウスX軸感度
const float PlayerController::MOUSE_Y_SENSITIVE = 100.0f;// マウスY軸感度
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
	, m_sensitive{ pPlayer->GetMouseSensitive() + 1.0f }// マウス感度:Player->GetMouseSensitive()のままだと最小値が0.0fになるので + 1.0fする
	, m_rotate{}// プレイヤーの回転
	, m_dashTime{ PlayerController::DASH_TIME }// ダッシュ時間
{
	RECT desktopRect;// スクリーンの解像度を取得
	const HWND hDesktop = GetDesktopWindow();// デスクトップウィンドウのハンドルを取得
	GetWindowRect(hDesktop, &desktopRect);// デスクトップウィンドウのサイズを取得
	// スクリーンの中央座標を計算
	int centerX = (desktopRect.right - desktopRect.left) / 2;// 中央X座標
	int centerY = (desktopRect.bottom - desktopRect.top) / 2;// 中央Y座標
	// マウス位置を中央に設定
	m_point = { centerX, centerY };// マウス座標
	SetCursorPos(m_point.x, m_point.y);// マウス位置を中央に設定
	// SetCursorPos(m_point.x, m_point.y)はint型のため、float型にキャストして保存
	m_lastMouseX = (float)(m_point.x);// マウスのX座標
	m_lastMouseY = (float)(m_point.y);// マウスのY座標
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
	m_velocity = Vector3::Zero;// 移動ベクトルをゼロにする
	m_velocity.y -= PlayerController::GRAVITY; // 重力をかける
	m_yaw = 0.0f;// yawを初期化
	m_pitch = 0.0f;	// pitchを初期化
	m_dash = PlayerController::MOVE_SPEED;// 通常速度
}
/*
*	@brief プレイヤーコントローラーの初期化
*	@details プレイヤーコントローラーの初期化(コモンリソースを渡す)
*	@param resources コモンリソース
*	@return なし
*/
void PlayerController::Initialize(CommonResources* resources) { m_pCommonResources = resources; }// 初期化
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
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();// キーボードステートトラッカーを取得する
	MoveStop();// 移動を止める
	POINT point;// マウスの座標
	GetCursorPos(&point);// マウスの位置を取得
	ScreenToClient(m_hWnd, &point);// ウィンドウ座標に変換
	int mouseX = point.x;// マウスのX座標
	int mouseY = point.y;// マウスのY座標
	// マウスの相対移動を計算
	float deltaX = static_cast<float>(mouseX - m_lastMouseX);// マウスのX座標の差分
	float deltaY = static_cast<float>(mouseY - m_lastMouseY);// マウスのY座標の差分
	// マウス感度の適用
	const float mouseSensitivity = PlayerController::MOUSE_SENSITIVE * m_sensitive;// マウス感度
	float x = deltaX * mouseSensitivity * PlayerController::MOUSE_X_SENSITIVE * elapsedTime;// マウスのX座標の差分
	float y = deltaY * mouseSensitivity * PlayerController::MOUSE_Y_SENSITIVE * elapsedTime;// マウスのY座標の差分
	// 回転
	m_yaw -= x;// 左右回転（yaw）
	m_pitch -= y;// 上下回転（pitch）
	float pitchLimit = XMConvertToRadians(m_pitchLimit);// ピッチの制限をラジアンに変換
	m_pitch = Clamp(m_pitch, -pitchLimit, pitchLimit);// 上下の回転制限
	// 前後移動
	if (kbTracker->lastState.W)// Wキーが押されているとき
	{
		// プレイヤーの方向に移動する
		m_velocity.x += m_pPlayer->GetPlayerDir().x;// X軸方向
		m_velocity.z += m_pPlayer->GetPlayerDir().z;// Z軸方向
	}
	if (kbTracker->lastState.S)// Sキーが押されているとき
	{
		// カメラの向きと逆の方向に移動する
		m_velocity.x -= m_pPlayer->GetPlayerDir().x;// X軸方向
		m_velocity.z -= m_pPlayer->GetPlayerDir().z;// Z軸方向
	}
	// カメラの右方向ベクトルを計算する(右を基準とする）
	Vector3 rightDirection;// 右方向ベクトル
	rightDirection.x = m_pPlayer->GetPlayerDir().z;// X軸方向
	rightDirection.z = -m_pPlayer->GetPlayerDir().x;// Z軸方向
	// 左右移動
	if (kbTracker->lastState.A)// Aキーが押されているとき
	{
		// カメラの左方向に移動する 
		m_velocity.x += rightDirection.x;// X軸方向
		m_velocity.z += rightDirection.z;// Z軸方向
	}
	if (kbTracker->lastState.D)// Dキーが押されているとき
	{
		// カメラの右方向に移動する
		m_velocity.x -= rightDirection.x;// X軸方向
		m_velocity.z -= rightDirection.z;// Z軸方向
	}
	// ダッシュ
	if (kbTracker->lastState.LeftShift)// 左shiftキーが押されているとき
	{
		if (m_dashTime > 0.0f)// ダッシュ時間が残っているとき
		{
			m_dash = PlayerController::DASH_SPEED;// ダッシュ速度
			m_dashTime -= elapsedTime;// ダッシュ時間を減らす
		}
	}
	else// 左shiftキーが押されていないとき
	{
		m_dash = PlayerController::MOVE_SPEED;// 通常速度
		m_dashTime += elapsedTime / 2;// ダッシュ時間を増やす
		if (m_dashTime >= PlayerController::DASH_TIME)// ダッシュ時間が上限を超えたとき
			m_dashTime = PlayerController::DASH_TIME;// ダッシュ時間の上限
	}
	if (m_velocity.LengthSquared() > 0)// 移動量があるとき
		m_velocity.Normalize();// 移動量を正規化する
	m_velocity *= m_dash * elapsedTime;// 移動量を補正する
	m_position += m_velocity;// 実際に移動する
	m_rotate = x;// 横回転を保存
	if (m_position.y <= Stage::STAGE_HEIGHT)m_position.y = Stage::STAGE_HEIGHT;// 地面の高さに制限
	// 地面の面積に制限
	if (m_position.x <= -Stage::STAGE_SIZE) m_position.x = -Stage::STAGE_SIZE;// 左端
	if (m_position.x >= Stage::STAGE_SIZE) m_position.x = Stage::STAGE_SIZE;// 右端
	if (m_position.z <= -Stage::STAGE_SIZE) m_position.z = -Stage::STAGE_SIZE;// 上端
	if (m_position.z >= Stage::STAGE_SIZE) m_position.z = Stage::STAGE_SIZE;// 下端
	SetCursorPos(m_point.x, m_point.y);	// マウス位置を中央に戻す
}
/*
*	@brief デバッグコマンド
*	@details デバッグ時のみ使用可能なチートコマンド
*	@param なし
*	@return なし
*/
void PlayerController::DebugCommand()
{
	const auto& kbTracker = m_pCommonResources->GetInputManager()->GetKeyboardTracker();// キーボードステートトラッカーを取得する
	auto& mTracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	if (mTracker->GetLastState().rightButton && m_pPlayer->GetisCheat() == false)// 右クリックで敵を一掃
	{
		m_pPlayer->SetisCheat(true);// チートフラグを立てる
		for (auto& enemy : m_pPlayer->GetEnemies()->GetEnemies())enemy->ApplyDamageToEnemy(1000);// 敵のHPを1000減らす（即死）
	}
	if (!mTracker->GetLastState().rightButton)// 右クリックされてないとき
		m_pPlayer->SetisCheat(false);	// チートコマンドを無効にする
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
	auto& mTracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	if (mTracker->GetLastState().leftButton &&// 左クリックされたとき
		m_pPlayer->GetBulletManager()->GetIsPlayerShoot() == false &&// 弾が発射されていないとき
		m_pPlayer->GetBulletManager()->GetPlayerBulletCount() > 0 && // 弾が残っているとき
		m_pPlayer->GetBulletManager()->GetIsReloading() == false)// リロード中でないとき
	{
		m_pPlayer->CreateBullet();// 弾を生成する
		m_pPlayer->GetBulletManager()->ConsumePlayerBullet();// 弾を消費する
	}
	if (!mTracker->GetLastState().leftButton)// 左クリックされていないとき
		m_pPlayer->GetBulletManager()->SetIsPlayerShoot(false);// 弾を撃ったフラグを下ろす
}
/*
*	@brief プレイヤーの弾をリロードする
*	@details Rキーで弾をリロードする
*	@param なし
*	@return なし
*/
void PlayerController::Reload()
{
	const auto& kbState = m_pCommonResources->GetInputManager()->GetKeyboardState();// キーボードステートトラッカーを取得する
	if (kbState.R)m_pPlayer->GetBulletManager()->SetIsReloading(true);// Rキーでリロードフラグを立てる
	if (m_pPlayer->GetBulletManager()->GetIsReloading())// リロード中のとき 
		m_pPlayer->GetBulletManager()->ReLoadPlayerBullet();// 弾を補充する
}
