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
	// ランダム値の最大値
	static const float RANDOM_MAX;
	// ランダム値の最小値
	static const float RANDOM_MIN;
	// ランダム回転の最大値
	static const float RANDOM_ROTATION_MAX;
	// ランダム回転の最小値
	static const float RANDOM_ROTATION_MIN;
	// 振幅
	static const float AMPLITUDE;
	// 周波数
	static const float FREQUENCY;
	// 移動速度
	static const float MOVE_SPEED;
	// 固定初速
	static const float FIXED_INITIAL_SPEED;
	// ノックバックの持続時間
	static const float KNOCKBACK_DURATION;
	// ノックバックの最大進行度
	static const float KNOCKBACK_PROGRESS_MAX;
	// ノックバックの減衰率
	static const float KNOCKBACK_DECAY_RATE;
	// 攻撃クールダウン時間
	static const float ATTACK_COOLDOWN;
	// 初期回転速度
	static const float INITIAL_ROTATION_SPEED;
	// 回転速度の分母
	static const float ROTATION_SPEED_DIVISOR;
	// 回転速度の最小値
	static const float ROTATION_SPEED_MIN;
	// 角度の閾値
	static const float ANGLE_THRESHOLD;
	// 表情を変える時間
	static const float CHANGE_FACE_TIME;
	// プレイヤーの方向に移動する際の速度スケールファクター
	static const float VELOCITY_SCALE_FACTOR;
	// 攻撃クールダウンの閾値
	static const float ATTACK_COOLDOWN_THRESHOLD;
	// 特殊攻撃のクールダウン時間
	static const float SPECIAL_ATTACK_COOLDOWN;
	// 攻撃間隔
	static const float ATTACK_INTERVAL;
	// 敵弾のサイズ
	static const float ENEMY_BULLET_SIZE;
	// ボス弾のサイズ
	static const float BOSS_BULLET_SIZE;
	// ラスボス弾のサイズ
	static const KnockBackTime KNOCKBACK_TIME;
	// ノックバック時間
	static const float LASTBOSS_BULLET_SIZE;
	// 内積をclampするときに使う値
	static const DotClamp DOT_CLAMP;
	// ===== ザコ敵専用 =====
	// 敵のスポーン半径
	static const float ENEMY_SPAWN_RADIUS;
	// 通常敵の半径
	static const float NORMAL_ENEMY_RADIUS;
	// 通常敵のダメージ
	static const float NORMAL_ENEMY_DAMAGE;
	// 敵のデスエフェクトのスケール
	static const float ENEMY_DEADEFFECT_SCALE;
	// 敵のスポーン開始時間
	static const float ENEMY_SPAWN_START_TIME;
	// 敵のスポーン間隔
	static const float ENEMY_SPAWN_INTERVAL;
	// ザコHPバーのオフセット
	static const DirectX::SimpleMath::Vector3 ENEMY_HPBAR_OFFSET;
	// ===== ボス共通 =====
	// ボスシールドの拡大速度
	static const float BOSS_SHIELD_SCALE_SPEED;
	// ボスシールドの攻撃クールダウン時間
	static const float BOSS_SHIELD_ATTACK_COOLDOWN;
	// ボスHPバーのスケール
	static const float BOSS_HPBAR_SCALE;
	// ボスの追跡速度
	static const float BOSS_CHASE_SPEED;
	// ボスのスポーン待機時間
	static const float BOSS_SPAWN_WAIT_TIME;

	// ===== ボス専用 =====
	// ボスの初期位置
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_POSITION;
	// ボスの初期スケール
	static const DirectX::SimpleMath::Vector3 INITIAL_BOSS_SCALE;
	// ボスの球体オフセット
	static const DirectX::SimpleMath::Vector3 BOSS_SPHERE_OFFSET;
	// ボスの頭のオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_HEAD_OFFSET;
	// ボスの左ガンのオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_LEFT_GUN_OFFSET;
	// ボスの右ガンのオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_RIGHT_GUN_OFFSET;
	// ボスHPバーのオフセット
	static const DirectX::SimpleMath::Vector3 BOSS_HPBAR_OFFSET;
	// 通常ボスの半径
	static const float NORMAL_BOSS_RADIUS;
	// ボスシールドの半径
	static const float BOSS_SHIELD_RADIUS;
	// ボスシールドのサイズ
	static const float BOSS_SHIELD_SIZE;
	// ボスがプレイヤーに与えるダメージ
	static const float BOSS_DAMAGE;
	// バリア破壊パーティクルのサイズ
	static const float BOSS_BARRIERBREAK_SIZE;
	// ボスのデスエフェクトのスケール
	static const float BOSS_DEADEFFECT_SCALE;


	// ===== ラスボス専用 =====
	// ボスの初期位置
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_POSITION;
	// ボスの初期スケール
	static const DirectX::SimpleMath::Vector3 INITIAL_LASTBOSS_SCALE;
	// ボスの球体オフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_SPHERE_OFFSET;
	// ボスの頭のオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HEAD_OFFSET;
	// ボスの左上ガンのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_LEFTUP_GUN_OFFSET;
	// ボスの右上ガンのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_RIGHTUP_GUN_OFFSET;
	// ボスの左下ガンのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_LEFTDOWN_GUN_OFFSET;
	// ボスの右下ガンのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_RIGHTDOWN_GUN_OFFSET;
	// ボスHPバーのオフセット
	static const DirectX::SimpleMath::Vector3 LASTBOSS_HPBAR_OFFSET;
	// 通常ボスの半径
	static const float NORMAL_LASTBOSS_RADIUS;
	// ボスシールドの半径
	static const float LASTBOSS_SHIELD_RADIUS;
	// ボスシールドのサイズ
	static const float LASTBOSS_SHIELD_SIZE;
	// ラスボスがプレイヤーに与えるダメージ
	static const float LASTBOSS_DAMAGE;
	// バリア破壊パーティクルのサイズ
	static const float LASTBOSS_BARRIERBREAK_SIZE;
	// ラスボスのデスエフェクトのスケール
	static const float LASTBOSS_DEADEFFECT_SCALE;

	// ===== 浮遊・特殊な初期状態など =====
	// 浮遊用の初速度
	static const DirectX::SimpleMath::Vector3 INITIAL_VELOCITY;

};