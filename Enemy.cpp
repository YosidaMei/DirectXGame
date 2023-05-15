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
	worldTransform_.translation_ = {0, 3, 20};
	
}

void Enemy::Update(){
	worldTransform_.UpdateMatrix();

	// キャラの速さ
	const float kCharaSpeed = 0.2f;
	const float kLeaveSpeed = 0.3f;
	/*
	// キャラの移動ベクトル
	Vector3 move = {0, 0, -kCharaSpeed};
	// 座標移動(ベクトルの加算)
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	*/

	switch (phase_) {
	case Phase::Approach:
	default:
		//移動
		worldTransform_.translation_.z -= kCharaSpeed;
		//既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//移動
		worldTransform_.translation_.x -= kLeaveSpeed;
		worldTransform_.translation_.y += kLeaveSpeed;

		break;
	}
}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);

}