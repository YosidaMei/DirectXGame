#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& posion,const Vector3& velocity) { 
	assert(model);
	model_ = model;
	m_textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = posion;
	velocity_ = velocity;
}


void PlayerBullet::Update() {

	worldTransform_.UpdateMatrix();
	//���W���ړ�������
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}


void PlayerBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);

}

void PlayerBullet::OnConllision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	// ���[���h���W������
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}