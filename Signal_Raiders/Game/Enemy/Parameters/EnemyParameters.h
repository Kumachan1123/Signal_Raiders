/*
	@file	EnemyParameters.h
	@brief	敵AI用パラメータークラス
*/
#pragma once
class EnemyParameters
{
public:// 定数用構造体
	// ノックバック開始から経過した時間
	struct KnockBackTime
	{
		float canAttackTime;    // 開始から一定経って攻撃できるようになる時間
		float endKnockTime;     // ノックバック処理をやめる時間
	};

	// 内積をclampするときに使う値
	struct DotClamp
	{
		float max;  // 最大値
		float min;  // 最小値
	};
public:// 定数

	// ===== 共通（ザコ・ボス両方に関係するもの） =====
	static const float RANDOM_MAX;// ランダム値の最大値
	static const float RANDOM_MIN;// ランダム値の最小値
	static const float RANDOM_ROTATION_MAX;// ランダム回転の最大値
	static const float RANDOM_ROTATION_MIN;// ランダム回転の最小値
	static const float AMPLITUDE;// 振幅
	static const float FREQUENCY; // 周波数
	static const float MOVE_SPEED;// 移動速度
	static const float FIXED_INITIAL_SPEED;// 固定初速
	static const float KNOCKBACK_DURATION;// ノックバックの持続時間
	static const float KNOCKBACK_PROGRESS_MAX;// ノックバックの最大進行度
	static const float KNOCKBACK_DECAY_RATE;// ノックバックの減衰率
	static const float ATTACK_COOLDOWN;// 攻撃クールダウン時間
	static const float INITIAL_ROTATION_SPEED;// 初期回転速度
	static const float ROTATION_SPEED_DIVISOR;// 回転速度の分母
	static const float ROTATION_SPEED_MIN;// 回転速度の最小値
	static const float ANGLE_THRESHOLD;// 角度の閾値
	static const float CHANGE_FACE_TIME;// 表情を変える時間
	static const float VELOCITY_SCALE_FACTOR;// プレイヤーの方向に移動する際の速度スケールファクター
	static const float ATTACK_COOLDOWN_THRESHOLD;// 攻撃クールダウンの閾値
	static const float SPECIAL_ATTACK_COOLDOWN;// 特殊攻撃のクールダウン時間
	static const float ATTACK_INTERVAL;// 攻撃間隔
	static const float ENEMY_BULLET_SIZE;// 敵弾のサイズ
	static const float BOSS_BULLET_SIZE;// ボス弾のサイズ
	static const KnockBackTime KNOCKBACK_TIME;// ノックバック時間
	static const DotClamp DOT_CLAMP;// 内積をclampするときに使う値
	// ===== ザコ敵専用 =====
	static const float ENEMY_SPAWN_RADIUS;// 敵のスポーン半径
	static const float NORMAL_ENEMY_RADIUS;// 通常敵の半径
	static const float NORMAL_ENEMY_DAMAGE;// 通常敵のダメージ
	static const float ENEMY_DEADEFFECT_SCALE;// 敵のデスエフェクトのスケール
	static const float ENEMY_SPAWN_START_TIME;// 敵のスポーン開始時間
	static const float ENEMY_SPAWN_INTERVAL; // 敵のスポーン間隔
	static const DirectX::SimpleMath::Vector3 ENEMY_HPBAR_OFFSET;// ザコHPバーのオフセット
	// ===== ボス共通 =====
	static const float BOSS_SHIELD_SCALE_SPEED;// ボスシールドの拡大速度
	static const float BOSS_SHIELD_RADIUS; // ボスシールドの半径
	static const float BOSS_SHIELD_ATTACK_COOLDOWN; // ボスシールドの攻撃クールダウン時間
	static const float NORMAL_BOSS_RADIUS;// 通常ボスの半径
	static const float BOSS_DAMAGE; // ボスのダメージ
	static const float BOSS_BULLET_ANGLE; // ボス弾の角度
	static const float BOSS_HPBAR_SCALE;// ボスHPバーのスケール
	static const float BOSS_CHASE_SPEED; // ボスの追跡速度
	static const float BOSS_DEADEFFECT_SCALE; // ボスのデスエフェクトのスケール
	static const float BOSS_SPAWN_WAIT_TIME; // ボスのスポーン待機時間

	// ===== ボス専用 =====
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_POSITION;// ボスの初期位置
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE; // ボスの初期スケール
	static const DirectX::SimpleMath::Vector3 BOSS_SPHERE_OFFSET;// ボスの球体オフセット
	static const DirectX::SimpleMath::Vector3 BOSS_HEAD_OFFSET;// ボスの頭のオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_LEFT_GUN_OFFSET;// ボスの左ガンのオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_RIGHT_GUN_OFFSET;// ボスの右ガンのオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_HPBAR_OFFSET;// ボスHPバーのオフセット

	// ===== ラスボス専用 =====
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_POSITION;// ボスの初期位置
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_SCALE; // ボスの初期スケール
	static const DirectX::SimpleMath::Vector3 LASTBOSS_SPHERE_OFFSET;// ボスの球体オフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HEAD_OFFSET;// ボスの頭のオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_LEFT_GUN_OFFSET;// ボスの左ガンのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_RIGHT_GUN_OFFSET;// ボスの右ガンのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HPBAR_OFFSET;// ボスHPバーのオフセット
	// ===== 浮遊・特殊な初期状態など =====
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY; // 浮遊用の初速度

};