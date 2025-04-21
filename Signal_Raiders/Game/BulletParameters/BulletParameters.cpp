/*
	@file	BulletParameters.cpp
	@brief	弾パラメータークラス
*/
#include "pch.h"
#include "BulletParameters.h"

using namespace DirectX::SimpleMath;

//======================================
// プレイヤー用の弾の定数
//======================================
const float BulletParameters::PLAYER_BULLET_SIZE = 0.1f;// 弾の大きさ

const float BulletParameters::COLLISION_RADIUS = 0.25f;// 弾の当たり判定の半径

const float BulletParameters::ADJUST_MOVE = 40.0f;// 移動量の補正値

const float BulletParameters::ADJUST_DIRECTION = 0.0375f;	// Directionを若干上方向にするための補正値

const Vector3  BulletParameters::INITIAL_POSITION = { 0.0f, -1.0f, 0.0f };	// 発射位置

const float  BulletParameters::PLAYER_BULLET_LIFETIME = 1.5;				// 寿命

const int  BulletParameters::DAMAGE = 10;						// 敵に与えるダメージ

const float  BulletParameters::RELOAD_INTERVAL = 0.1f;				// リロード間隔
//======================================
// 敵用の弾の定数
//======================================
const float  BulletParameters::ENEMY_BULLET_LIFETIME = 5.0f;			// 寿命

const float  BulletParameters::SPIRAL_BULLET_LIFETIME = 15.0f;			// 寿命

const float  BulletParameters::STRAIGHT_BULLET_SPEED = 10.0f;			// 直線弾の速度

const float  BulletParameters::STRAIGHT_ADJUST_DIRECTION = 0.05f;	// Directionを若干下にするための補正値

const float  BulletParameters::VERTICAL_BULLET_SPEED = 50.0f;			// 垂直弾の速度

const Vector3  BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY = { 0.0f, -0.15f, 0.0f };	// 着弾前の速度

const float  BulletParameters::VERTICAL_BULLET_LANDING_POSITION = 0.5f;	// 垂直弾の着弾位置

const float  BulletParameters::SPECIAL_ATTACK_WAIT_TIME = 5.0f;	// 特殊攻撃の待機時間


//======================================
// 共通の定数（プレイヤー・敵共通で使うもの）
//======================================
const float  BulletParameters::BULLET_ROTATION_SPEED = 6.0f;// 弾の自転速度(度)

const Plane  BulletParameters::SHADOW_PLANE = DirectX::SimpleMath::Plane(0.0f, 1.0f, 0.0f, 0.01f);// 影の平面

const float  BulletParameters::ADJUST_HIT_VOLUME = 0.8f;// ヒットSEボリューム調整

const float  BulletParameters::ANGLE_MIN = 0.0f;// 角度の最小値

const float  BulletParameters::ANGLE_MAX = 360.0f;// 角度の最大値

const float  BulletParameters::DELETE_BULLET_POSITION = -0.25f;// 弾の削除位置

const int  BulletParameters::MAX_PLAYER_BULLET_COUNT = 50;// プレイヤーの弾の最大数

const float  BulletParameters::HIT_VOLUME = 0.8f;// ヒット音のボリューム

