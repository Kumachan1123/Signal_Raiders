/*
	@file	EnemyParameters.cpp
	@brief	敵AI用パラメータークラス
*/
#include "pch.h"
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

const float EnemyParameters::RANDOM_MAX = 7.0f;// ランダムの最大値

const float EnemyParameters::RANDOM_MIN = 5.5f;// ランダムの最小値

const float EnemyParameters::AMPLITUDE = 2.0f;// 振幅

const float EnemyParameters::FREQUENCY = 0.5f;// 周波数

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

const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;// フレームごとに弾を発射しなくするための攻撃閾値

const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f,0.5f,0.0f };// 浮遊の初期速度

const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f,2.0f,2.0f };// ボスの初期スケール

const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };// ノックバック開始から経過した時間

const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };// 内積をclampするときに使う値


