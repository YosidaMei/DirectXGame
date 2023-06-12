#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::UpdateMatrix() {
	//スケール回転平行移動を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//親があれば親のワールド行列をかける
	if (parent_) {
		matWorld_= Multiply(matWorld_, parent_->matWorld_);
	}
	//転送
	TransferMatrix();
}