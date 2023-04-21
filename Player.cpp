#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	m_textureHandle_ = textureHandle;
	worldTransform_.Initialize();

}

void Player::Update() { 

	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);

}