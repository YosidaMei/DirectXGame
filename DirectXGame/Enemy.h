#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"
#include "PlayerBullet.h"
#include <list>
#include "MathFunction.h"
#include "EnemyBullet.h"

//自機クラスの前方宣言
class Player;

enum class Phase {
	Approach, // 接近
	Leave,    // 離脱
};
class Enemy {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	// 衝突を検知したら呼び出される関数
	void OnConllision();

	// テクスチャハンドル
	uint32_t m_textureHandle_ = 0u;

	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// キーボード入力
	Input* input_ = nullptr;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//発射感覚
	static const int kFireInterval = 100;
	//発射タイマー
	int32_t fireTimer = 0;

	void approachInitialize();

	// 自キャラ
	Player* player_ = nullptr;
	void SetPlayer(Player* player) { player_ = player; }

	const float radius_ = 2;

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	private:
	std::list<EnemyBullet*> bullets_;
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
};

