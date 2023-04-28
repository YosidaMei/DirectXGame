#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& posion) { 
	assert(model);
	model_ = model;
	m_textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = posion;
}


void PlayerBullet::Update() {

	worldTransform_.UpdateMatrix();

}


void PlayerBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);

}