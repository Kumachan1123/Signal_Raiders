/*
	@file	EnemyParameters.cpp
	@brief	敵AI用パラメータークラス
*/
#include <pch.h>
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

// 定数
// ==============================
// 共通（ボス・ザコ両方）
// ==============================
// ランダムの最大値
const float EnemyParameters::RANDOM_MAX = 7.0f;
// ランダムの最小値
const float EnemyParameters::RANDOM_MIN = 5.5f;
// ランダム回転の最大値
const float EnemyParameters::RANDOM_ROTATION_MAX = 2.0f;
// ランダム回転の最小値
const float EnemyParameters::RANDOM_ROTATION_MIN = 0.5f;
// 振幅
const float EnemyParameters::AMPLITUDE = 2.0f;
// 周波数
const float EnemyParameters::FREQUENCY = 0.5f;
// 移動速度
const float EnemyParameters::MOVE_SPEED = 20.0f;
// 固定初速
const float EnemyParameters::FIXED_INITIAL_SPEED = 20.0f;
// ノックバックの持続時間
const float EnemyParameters::KNOCKBACK_DURATION = 2.0f;
// ノックバックの最大進行度
const float EnemyParameters::KNOCKBACK_PROGRESS_MAX = 2.0f;
// ノックバックの減衰率
const float EnemyParameters::KNOCKBACK_DECAY_RATE = -3.0f;
// 攻撃クールダウン時間
const float EnemyParameters::ATTACK_COOLDOWN = 3.0f;
// 初期回転速度
const float EnemyParameters::INITIAL_ROTATION_SPEED = 1.0f;
// 回転速度の分母
const float EnemyParameters::ROTATION_SPEED_DIVISOR = 10.0f;
// 回転速度の最小値
const float EnemyParameters::ROTATION_SPEED_MIN = 0.24f;
// 角度の閾値
const float EnemyParameters::ANGLE_THRESHOLD = 0.1f;
// 表情を変える時間
const float EnemyParameters::CHANGE_FACE_TIME = 2.0f;
// プレイヤーの方向に移動する際の速度スケールファクター
const float EnemyParameters::VELOCITY_SCALE_FACTOR = 5.0f;
// 攻撃クールダウンの閾値
const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;
// 特殊攻撃クールダウン時間
const float EnemyParameters::SPECIAL_ATTACK_COOLDOWN = 30.0f;
// 攻撃間隔
const float EnemyParameters::ATTACK_INTERVAL = 1.0f;
// 弾のサイズ
const float EnemyParameters::ENEMY_BULLET_SIZE = 0.15f;
// ボス弾のサイズ
const float EnemyParameters::BOSS_BULLET_SIZE = 0.25f;
// ラスボス弾のサイズ
const float EnemyParameters::LASTBOSS_BULLET_SIZE = 0.75f;
// ノックバック時間
const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };
// ノックバック時間のクランプ
const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };


// ==============================
// ザコ敵専用
// ==============================
// 敵のスポーン半径
const float EnemyParameters::ENEMY_SPAWN_RADIUS = 50.0f;
// 通常敵の半径
const float EnemyParameters::NORMAL_ENEMY_RADIUS = 1.5f;
// 通常敵のダメージ
const float EnemyParameters::NORMAL_ENEMY_DAMAGE = 1.0f;
// 通常敵のHP
const float EnemyParameters::ENEMY_DEADEFFECT_SCALE = 3.0f;
// 敵のスポーン開始時間
const float EnemyParameters::ENEMY_SPAWN_START_TIME = 5.0f;
// 敵のスポーン間隔
const float EnemyParameters::ENEMY_SPAWN_INTERVAL = 0.5f;
// ザコHPバーのオフセット
const Vector3 EnemyParameters::ENEMY_HPBAR_OFFSET{ 0.0f, 1.0f, 0.0f };


// ==============================
// ボス共通
// ==============================
// ボスシールドの拡大速度
const float EnemyParameters::BOSS_SHIELD_SCALE_SPEED = 0.2f;
// ボスシールドの攻撃クールダウン時間
const float EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN = 0.75f;
// ボスHPバーのスケール
const float EnemyParameters::BOSS_HPBAR_SCALE = 3.0f;
// ボスの追跡速度
const float EnemyParameters::BOSS_CHASE_SPEED = 2.0f;
// ボスのスポーン待機時間
const float EnemyParameters::BOSS_SPAWN_WAIT_TIME = 3.0f;

// ===============================
// ボス専用
// ===============================
// ボスの初期位置
const Vector3 EnemyParameters::INITIAL_BOSS_POSITION{ 0.0f, 5.0f, 0.0f };
// ボスの初期スケール
const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f, 2.0f, 2.0f };
// ボスの球体オフセット
const Vector3 EnemyParameters::BOSS_SPHERE_OFFSET{ 0.0f, 0.5f, 0.0f };
// ボスの頭のオフセット
const Vector3 EnemyParameters::BOSS_HEAD_OFFSET{ 0.0f,2.0f, 3.0f };
// ボスの左ガンのオフセット
const Vector3 EnemyParameters::BOSS_LEFT_GUN_OFFSET{ -2.5f, -0.50f, 3.0f };
// ボスの右ガンのオフセット
const Vector3 EnemyParameters::BOSS_RIGHT_GUN_OFFSET{ 2.5f, -0.50f, 3.0f };
// ボスHPバーのオフセット
const Vector3 EnemyParameters::BOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };
// 通常ボスの半径
const float EnemyParameters::NORMAL_BOSS_RADIUS = 3.0f;
// ボスシールドの半径
const float EnemyParameters::BOSS_SHIELD_RADIUS = 5.5f;
// ボスシールドのサイズ
const float EnemyParameters::BOSS_SHIELD_SIZE = 3.0f;
// ボスがプレイヤーに与えるダメージ
const float EnemyParameters::BOSS_DAMAGE = 5.0f;
// バリア破壊パーティクルのサイズ
const float EnemyParameters::BOSS_BARRIERBREAK_SIZE = 1.0f;
// ボスのデスエフェクトのスケール
const float EnemyParameters::BOSS_DEADEFFECT_SCALE = 10.0f;

// ================================
// ラスボス専用
// ================================
// ボスの初期位置
const Vector3 EnemyParameters::INITIAL_LASTBOSS_POSITION{ 0.0f, 10.0f, 0.0f };
// ボスの初期スケール
const Vector3 EnemyParameters::INITIAL_LASTBOSS_SCALE{ 1.0f, 1.0f, 1.0f };
// ラスボスの球体オフセット
const Vector3 EnemyParameters::LASTBOSS_SPHERE_OFFSET{ 0.0f, 0.5f, 0.0f };
// ラスボスの頭のオフセット
const Vector3 EnemyParameters::LASTBOSS_HEAD_OFFSET{ 0.0f, -1.0f, 3.0f };
// ラスボスの左ガンのオフセット
const Vector3 EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET{ -9.5f, 4.50f, 9.0f };
// ラスボスの右ガンのオフセット
const Vector3 EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET{ 9.5f, 4.50f, 9.0f };
// ラスボスの左下ガンのオフセット
const Vector3 EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET{ -9.5f, -3.50f, 9.0f };
// ラスボスの右下ガンのオフセット
const Vector3 EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET{ 9.5f, -3.50f, 9.0f };
// ラスボスHPバーのオフセット
const Vector3 EnemyParameters::LASTBOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };
// 通常ボスの半径
const float EnemyParameters::NORMAL_LASTBOSS_RADIUS = 8.5f;
// ボスシールドの半径
const float EnemyParameters::LASTBOSS_SHIELD_RADIUS = 20.5f;
// ボスシールドのサイズ
const float EnemyParameters::LASTBOSS_SHIELD_SIZE = 10.0f;
// ラスボスがプレイヤーに与えるダメージ
const float EnemyParameters::LASTBOSS_DAMAGE = 10.0f;
// バリア破壊パーティクルのサイズ
const float EnemyParameters::LASTBOSS_BARRIERBREAK_SIZE = 2.0f;
// ラスボスのデスエフェクトのスケール
const float EnemyParameters::LASTBOSS_DEADEFFECT_SCALE = 15.0f;



// ==============================
// 演出・特殊用途
// ==============================
// 敵の初期位置
const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f, 0.5f, 0.0f };
