/*
	@file	EnemyParameters.cpp
	@brief	敵AI用パラメータークラス
*/
#include "pch.h"
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

// 定数

const float EnemyParameters::ENEMY_SPAWN_RADIUS = 50.0f;// ザコ敵が出現する半径

const float EnemyParameters::RANDOM_MAX = 7.0f;// ランダムの最大値

const float EnemyParameters::RANDOM_MIN = 5.5f;// ランダムの最小値

const float EnemyParameters::RANDOM_ROTATION_MAX = 2.0f;// ランダムの最大値

const float EnemyParameters::RANDOM_ROTATION_MIN = 0.5f;// ランダムの最小値

const float EnemyParameters::AMPLITUDE = 2.0f;// 振幅

const float EnemyParameters::FREQUENCY = 0.5f;// 周波数

const float EnemyParameters::MOVE_SPEED = 20.0f;// 移動速度

const float EnemyParameters::FIXED_INITIAL_SPEED = 20.0f;// ノックバックの初速度

const float EnemyParameters::KNOCKBACK_DURATION = 2.0f;// ノックバックの長さ（秒）

const float EnemyParameters::KNOCKBACK_PROGRESS_MAX = 2.0f;// ノックバック進行度の最大値

const float EnemyParameters::KNOCKBACK_DECAY_RATE = -3.0f;// ノックバックの減衰率

const float EnemyParameters::ATTACK_COOLDOWN = 3.0f;// 攻撃のクールダウンタイム

const float EnemyParameters::INITIAL_ROTATION_SPEED = 1.0f;// 回転速度

const float EnemyParameters::ROTATION_SPEED_DIVISOR = 10.0f;// 回転速度に除算するときに使う値

const float EnemyParameters::ROTATION_SPEED_MIN = 0.24f;// 回転速度の最小値

const float EnemyParameters::ANGLE_THRESHOLD = 0.1f;// 角度のしきい値

const float EnemyParameters::CHANGE_FACE_TIME = 2.0f;// 表情を変える時間

const float EnemyParameters::VELOCITY_SCALE_FACTOR = 5.0f;// 正規化した速度に乗算する値

const float EnemyParameters::BOSS_SHIELD_SCALE_SPEED = 0.2f;// ボスのシールドが大きくなる速さ

const float EnemyParameters::BOSS_SHIELD_RADIUS = 5.5f;// シールド展開時のボスの境界球の半径

const float EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN = 0.75f;// シールド展開時のボスの攻撃間隔

const float EnemyParameters::NORMAL_BOSS_RADIUS = 2.5f;// 通常時のボスの境界球の半径

const float EnemyParameters::NORMAL_ENEMY_RADIUS = 1.5f;// ザコ敵の境界球の半径

const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;// フレームごとに弾を発射しなくするための攻撃閾値

const float EnemyParameters::NORMAL_ENEMY_DAMAGE = 1.0f;// ザコ敵がプレイヤーに与えるダメージ

const float EnemyParameters::ATTACK_INTERVAL = 1.0f;// ザコ敵の攻撃インターバル

const float EnemyParameters::BOSS_DAMAGE = 5.0f;// ボスがプレイヤーに与えるダメージ

const float EnemyParameters::BOSS_BULLET_ANGLE = 30.0f;// ボスの弾の角度

const float EnemyParameters::BOSS_HPBAR_SCALE = 3.0f;// ボスのHPバーのスケール

const float EnemyParameters::BOSS_CHASE_SPEED = 2.0f;// ボスの追跡速度

const float EnemyParameters::BOSS_DEADEFFECT_SCALE = 10.0f;// ボスの死亡エフェクトのスケール

const float EnemyParameters::ENEMY_DEADEFFECT_SCALE = 3.0f;// ザコ敵の死亡エフェクトのスケール

const float EnemyParameters::ENEMY_SPAWN_START_TIME = 5.0f;// 敵が出現し始める時間

const float EnemyParameters::ENEMY_SPAWN_INTERVAL = 0.5f;// 敵が出現する間隔

const float EnemyParameters::ENEMY_BULLET_SIZE = 0.15f;// 敵の弾の大きさ

const float EnemyParameters::BOSS_BULLET_SIZE = 0.25f;// ボスの弾の大きさ

const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f,0.5f,0.0f };// 浮遊の初期速度

const Vector3 EnemyParameters::INITIAL_BOSS_POSITION{ 0.0f,10.0f,0.0f };// ボスの初期位置

const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f,2.0f,2.0f };// ボスの初期スケール

const Vector3 EnemyParameters::BOSS_SPHERE_OFFSET{ 0.0f,0.5f,0.0f };// ボスの境界球のオフセット

const Vector3 EnemyParameters::BOSS_HEAD_OFFSET{ 0.0f, 2.5f, 3.0f };// ボスの頭の砲口のオフセット

const Vector3 EnemyParameters::BOSS_LEFT_GUN_OFFSET{ -2.5f, 1.0f, 3.0f };// 上の座標

const Vector3 EnemyParameters::BOSS_RIGHT_GUN_OFFSET{ 2.5f, 1.0f, 3.0f };// 右の座標

const Vector3 EnemyParameters::BOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };// HPバーのオフセット

const Vector3 EnemyParameters::ENEMY_HPBAR_OFFSET{ 0.0f, 1.0f, 0.0f };// HPバーのオフセット

const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };// ノックバック開始から経過した時間

const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };// 内積をclampするときに使う値


