#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model) {
	// Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	m_textureHandle_ = TextureManager::Load("AL3wadoru.png");
	worldTransform_.Initialize();
	
}

void Enemy::Update(){
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.y = 3;
	// キャラの速さ
	const float kCharaSpeed = 0.2f;
	// キャラの移動ベクトル
	Vector3 move = {0, 0, -kCharaSpeed};

	// 座標移動(ベクトルの加算)
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);

}