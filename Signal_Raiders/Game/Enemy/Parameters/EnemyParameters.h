/*
	@file	EnemyParameters.h
	@brief	敵AI用パラメータークラス
*/
#pragma once
class EnemyParameters
{
public:// 定数用構造体
	struct KnockBackTime// ノックバック開始から経過した時間
	{
		// 開始から一定経って攻撃できるようになる時間
		float canAttackTime;
		// ノックバック処理をやめる時間
		float endKnockTime;
	};
	struct DotClamp// 内積をclampするときに使う値
	{
		// 最大値
		float max;
		// 最小値
		float min;
	};
public:// 定数
	static const float ENEMY_SPAWN_RADIUS;	// ザコ敵が出現する半径
	static const float RANDOM_MAX;  // ランダムの最大値
	static const float RANDOM_MIN;  // ランダムの最小値
	static const float RANDOM_ROTATION_MAX;  // ランダムの最大値
	static const float RANDOM_ROTATION_MIN;  // ランダムの最小値
	static const float AMPLITUDE;  // 振幅
	static const float FREQUENCY;  // 周波数
	static const float MOVE_SPEED;  // 移動速度
	static const float FIXED_INITIAL_SPEED;// ノックバックの初速度
	static const float KNOCKBACK_DURATION;	// ノックバックの長さ（秒）
	static const float KNOCKBACK_PROGRESS_MAX; // ノックバック進行度の最大値
	static const float KNOCKBACK_DECAY_RATE;  // ノックバックの減衰率
	static const float ATTACK_COOLDOWN;  // 攻撃のクールダウンタイム
	static const float INITIAL_ROTATION_SPEED; // 回転速度
	static const float ROTATION_SPEED_DIVISOR; // 回転速度に除算するときに使う値
	static const float ROTATION_SPEED_MIN; // 回転速度の最小値
	static const float ANGLE_THRESHOLD; // 角度のしきい値
	static const float CHANGE_FACE_TIME;	// 表情を変える時間
	static const float VELOCITY_SCALE_FACTOR;	// 正規化した速度に乗算する値
	static const float BOSS_SHIELD_SCALE_SPEED;	// ボスのシールドが大きくなる速さ
	static const float BOSS_SHIELD_RADIUS;	// シールド展開時のボスの境界球の半径
	static const float BOSS_SHIELD_ATTACK_COOLDOWN;	// シールド展開時のボスの攻撃間隔
	static const float ATTACK_COOLDOWN_THRESHOLD;	// フレームごとに弾を発射しなくするための攻撃閾値
	static const float NORMAL_ENEMY_RADIUS;	// ザコ敵の境界球の半径
	static const float NORMAL_BOSS_RADIUS;	// 通常時のボスの境界球の半径
	static const float NORMAL_ENEMY_DAMAGE;	// ザコ敵がプレイヤーに与えるダメージ
	static const float ATTACK_INTERVAL;	// 敵の攻撃インターバル
	static const float BOSS_DAMAGE;	// ボスがプレイヤーに与えるダメージ
	static const float BOSS_BULLET_ANGLE;	// ボスの弾の角度
	static const float BOSS_HPBAR_SCALE;	// ボスのHPバーのスケール
	static const float BOSS_CHASE_SPEED;	// ボスの追跡速度
	static const float BOSS_DEADEFFECT_SCALE;	// ボスの死亡エフェクトのスケール
	static const float ENEMY_DEADEFFECT_SCALE;	// ザコ敵の死亡エフェクトのスケール
	static const float ENEMY_SPAWN_START_TIME;	// 敵が出現し始める時間
	static const float ENEMY_SPAWN_INTERVAL;// 敵が出現する間隔
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY; // 浮遊の初期速度
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_POSITION; // ボスの初期位置
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE; // ボスの初期スケール
	static const DirectX::SimpleMath::Vector3 BOSS_SPHERE_OFFSET; // ボスの境界球のオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_HEAD_OFFSET;	// ボスの頭の砲口のオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_LEFT_GUN_OFFSET;	// ボスの左の砲口のオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_RIGHT_GUN_OFFSET;// ボスの右の砲口のオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_HPBAR_OFFSET;	// ボスのシールドのオフセット
	static const KnockBackTime KNOCKBACK_TIME;// ノックバック開始から経過した時間
	static const DotClamp DOT_CLAMP;// 内積をclampするときに使う値

};