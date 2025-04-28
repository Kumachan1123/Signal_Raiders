/*
	@file	EnemyParameters.cpp
	@brief	敵AI用パラメータークラス
*/
#include "pch.h"
#include "EnemyParameters.h"

using namespace DirectX::SimpleMath;

// 定数
// ==============================
// 共通（ボス・ザコ両方）
// ==============================
const float EnemyParameters::RANDOM_MAX = 7.0f;
const float EnemyParameters::RANDOM_MIN = 5.5f;
const float EnemyParameters::RANDOM_ROTATION_MAX = 2.0f;
const float EnemyParameters::RANDOM_ROTATION_MIN = 0.5f;
const float EnemyParameters::AMPLITUDE = 2.0f;
const float EnemyParameters::FREQUENCY = 0.5f;
const float EnemyParameters::MOVE_SPEED = 20.0f;
const float EnemyParameters::FIXED_INITIAL_SPEED = 20.0f;
const float EnemyParameters::KNOCKBACK_DURATION = 2.0f;
const float EnemyParameters::KNOCKBACK_PROGRESS_MAX = 2.0f;
const float EnemyParameters::KNOCKBACK_DECAY_RATE = -3.0f;
const float EnemyParameters::ATTACK_COOLDOWN = 3.0f;
const float EnemyParameters::INITIAL_ROTATION_SPEED = 1.0f;
const float EnemyParameters::ROTATION_SPEED_DIVISOR = 10.0f;
const float EnemyParameters::ROTATION_SPEED_MIN = 0.24f;
const float EnemyParameters::ANGLE_THRESHOLD = 0.1f;
const float EnemyParameters::CHANGE_FACE_TIME = 2.0f;
const float EnemyParameters::VELOCITY_SCALE_FACTOR = 5.0f;
const float EnemyParameters::ATTACK_COOLDOWN_THRESHOLD = 0.1f;
const float EnemyParameters::SPECIAL_ATTACK_COOLDOWN = 30.0f;
const float EnemyParameters::ATTACK_INTERVAL = 1.0f;
const float EnemyParameters::ENEMY_BULLET_SIZE = 0.15f;
const float EnemyParameters::BOSS_BULLET_SIZE = 0.25f;
const float EnemyParameters::LASTBOSS_BULLET_SIZE = 0.75f;

const EnemyParameters::KnockBackTime EnemyParameters::KNOCKBACK_TIME = { 0.3f, 1.0f };
const EnemyParameters::DotClamp EnemyParameters::DOT_CLAMP = { 1.0f, -1.0f };


// ==============================
// ザコ敵専用
// ==============================
const float EnemyParameters::ENEMY_SPAWN_RADIUS = 50.0f;
const float EnemyParameters::NORMAL_ENEMY_RADIUS = 1.5f;
const float EnemyParameters::NORMAL_ENEMY_DAMAGE = 1.0f;
const float EnemyParameters::ENEMY_DEADEFFECT_SCALE = 3.0f;
const float EnemyParameters::ENEMY_SPAWN_START_TIME = 5.0f;
const float EnemyParameters::ENEMY_SPAWN_INTERVAL = 0.5f;
const Vector3 EnemyParameters::ENEMY_HPBAR_OFFSET{ 0.0f, 1.0f, 0.0f };


// ==============================
// ボス共通
// ==============================
const float EnemyParameters::BOSS_SHIELD_SCALE_SPEED = 0.2f;
const float EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN = 0.75f;
const float EnemyParameters::BOSS_HPBAR_SCALE = 3.0f;
const float EnemyParameters::BOSS_CHASE_SPEED = 2.0f;
const float EnemyParameters::BOSS_DEADEFFECT_SCALE = 10.0f;
const float EnemyParameters::BOSS_SPAWN_WAIT_TIME = 3.0f;

// ===============================
// ボス専用
// ===============================
const Vector3 EnemyParameters::INITIAL_BOSS_POSITION{ 0.0f, 10.0f, 0.0f };
const Vector3 EnemyParameters::INITIAL_BOSS_SCALE{ 2.0f, 2.0f, 2.0f };
const Vector3 EnemyParameters::BOSS_SPHERE_OFFSET{ 0.0f, 0.5f, 0.0f };
const Vector3 EnemyParameters::BOSS_HEAD_OFFSET{ 0.0f,2.0f, 3.0f };
const Vector3 EnemyParameters::BOSS_LEFT_GUN_OFFSET{ -2.5f, -0.50f, 3.0f };
const Vector3 EnemyParameters::BOSS_RIGHT_GUN_OFFSET{ 2.5f, -0.50f, 3.0f };
const Vector3 EnemyParameters::BOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };
const float EnemyParameters::NORMAL_BOSS_RADIUS = 2.5f;
const float EnemyParameters::BOSS_SHIELD_RADIUS = 5.5f;
const float EnemyParameters::BOSS_DAMAGE = 5.0f;

// ================================
// ラスボス専用
// ================================
const Vector3 EnemyParameters::INITIAL_LASTBOSS_POSITION{ 0.0f, 10.0f, 0.0f };
const Vector3 EnemyParameters::INITIAL_LASTBOSS_SCALE{ 1.0f, 1.0f, 1.0f };
const Vector3 EnemyParameters::LASTBOSS_SPHERE_OFFSET{ 0.0f, 0.5f, 0.0f };
const Vector3 EnemyParameters::LASTBOSS_HEAD_OFFSET{ 0.0f, -1.0f, 3.0f };
const Vector3 EnemyParameters::LASTBOSS_LEFTUP_GUN_OFFSET{ -9.5f, 4.50f, 7.0f };
const Vector3 EnemyParameters::LASTBOSS_RIGHTUP_GUN_OFFSET{ 9.5f, 4.50f, 7.0f };
const Vector3 EnemyParameters::LASTBOSS_LEFTDOWN_GUN_OFFSET{ -9.5f, -3.50f, 7.0f };
const Vector3 EnemyParameters::LASTBOSS_RIGHTDOWN_GUN_OFFSET{ 9.5f, -3.50f, 7.0f };
const Vector3 EnemyParameters::LASTBOSS_HPBAR_OFFSET{ 0.0f, 3.0f, 0.0f };
const float EnemyParameters::NORMAL_LASTBOSS_RADIUS = 10.5f;// 通常ボスの半径
const float EnemyParameters::LASTBOSS_SHIELD_RADIUS = 20.5f; // ボスシールドの半径
const float EnemyParameters::LASTBOSS_DAMAGE = 10.0f;



// ==============================
// 演出・特殊用途
// ==============================
const Vector3 EnemyParameters::INITIAL_VELOCITY{ 0.0f, 0.5f, 0.0f };
