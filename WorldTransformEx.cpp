#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::UpdateMatrix() {
	//�X�P�[����]���s�ړ����v�Z����
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//�]��
	TransferMatrix();
}