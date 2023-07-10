#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(WorldTransform& worldTransform) { 
	//���[���h�g�����X�t�H�[��
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.Initialize();
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 120;
	viewProjection_.Initialize();
	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();

}

void RailCamera::Update() {

	////�ړ�
	//// �L�����̈ړ��x�N�g��
	//Vector3 move = {0, 0, 0};
	//// �L�����̑���
	//const float kCharaSpeed = 0.2f;
	//// �����������ňړ��x�N�g����ύX(���E)
	//if (input_->PushKey(DIK_G)) {
	//	move.x -= kCharaSpeed;
	//} else if (input_->PushKey(DIK_J)) {
	//	move.x += kCharaSpeed;
	//}
	//// �㉺
	//if (input_->PushKey(DIK_H)) {
	//	move.y -= kCharaSpeed;
	//} else if (input_->PushKey(DIK_Y)) {
	//	move.y += kCharaSpeed;
	//}
	////Z��
	//if (input_->PushKey(DIK_V)) {
	//	move.z -= kCharaSpeed;
	//}
	//else if (input_->PushKey(DIK_B)) {
	//	move.z += kCharaSpeed;
	//}
	//// ���W�ړ�(�x�N�g���̉��Z)
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//worldTransform_.translation_.x += move.x;
	//worldTransform_.translation_.y += move.y;
	//worldTransform_.translation_.z += move.z;

	////��]
	////  ��]�̑���
	//const float kRotSpeed = 0.02f;
	//// �����������ňړ��x�N�g����ύX
	//if (input_->PushKey(DIK_T)) {
	//	worldTransform_.rotation_.y -= kRotSpeed;
	//} else if (input_->PushKey(DIK_U)) {
	//	worldTransform_.rotation_.y += kRotSpeed;
	//}

	//�J�����ړ�
	const float rotationSpeed = 0.005f;
	// �����������ňړ��x�N�g����ύX(���E)
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= rotationSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += rotationSpeed;
	}
	

	//���[���h�s��Čv�Z
	worldTransform_.UpdateMatrix();

	//���[���h�s�񂩂�r���[�s���
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//** �f�o�b�O�J���� **//
	ImGui::Begin("Camera");
	//ImGui::InputFloat3("railCamera", )
	ImGui::Text(
	    "RailCamera PR : %f %f %f %f %f %f",
		worldTransform_.translation_.x,worldTransform_.translation_.y, worldTransform_.translation_.z,
		worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	ImGui::End();
}