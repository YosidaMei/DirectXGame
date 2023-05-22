#include "EnemyBullet.h"
#include <assert.h>

void EnemyBullet::Initialize(Model* model,const Vector3& position,const Vector3& velocity) {
	assert(model);
	model_ = model;
	m_textureHandle_ = TextureManager::Load("hosi_kao1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	// ç¿ïWÇà⁄ìÆÇ≥ÇπÇÈ
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);
}
