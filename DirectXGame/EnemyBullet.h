#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"

class EnemyBullet {
public:
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }

	// 衝突を検知したら呼び出される関数
	void OnConllision();

	Vector3 GetWorldPosition();

	const float radius_ = 2;

	private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t m_textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
