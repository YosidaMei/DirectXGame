#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) { 
	// Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0}; 
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

}
