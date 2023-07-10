#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(WorldTransform& worldTransform) { 
	//ワールドトランスフォーム
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 120;
	viewProjection_.Initialize();
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void RailCamera::Update() {

	////移動
	//// キャラの移動ベクトル
	//Vector3 move = {0, 0, 0};
	//// キャラの速さ
	//const float kCharaSpeed = 0.2f;
	//// 押した方向で移動ベクトルを変更(左右)
	//if (input_->PushKey(DIK_G)) {
	//	move.x -= kCharaSpeed;
	//} else if (input_->PushKey(DIK_J)) {
	//	move.x += kCharaSpeed;
	//}
	//// 上下
	//if (input_->PushKey(DIK_H)) {
	//	move.y -= kCharaSpeed;
	//} else if (input_->PushKey(DIK_Y)) {
	//	move.y += kCharaSpeed;
	//}
	////Z軸
	//if (input_->PushKey(DIK_V)) {
	//	move.z -= kCharaSpeed;
	//}
	//else if (input_->PushKey(DIK_B)) {
	//	move.z += kCharaSpeed;
	//}
	//// 座標移動(ベクトルの加算)
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//worldTransform_.translation_.x += move.x;
	//worldTransform_.translation_.y += move.y;
	//worldTransform_.translation_.z += move.z;

	////回転
	////  回転の速さ
	//const float kRotSpeed = 0.02f;
	//// 押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_T)) {
	//	worldTransform_.rotation_.y -= kRotSpeed;
	//} else if (input_->PushKey(DIK_U)) {
	//	worldTransform_.rotation_.y += kRotSpeed;
	//}

	//カメラ移動
	const float rotationSpeed = 0.005f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= rotationSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += rotationSpeed;
	}
	

	//ワールド行列再計算
	worldTransform_.UpdateMatrix();

	//ワールド行列からビュー行列へ
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//** デバッグカメラ **//
	ImGui::Begin("Camera");
	//ImGui::InputFloat3("railCamera", )
	ImGui::Text(
	    "RailCamera PR : %f %f %f %f %f %f",
		worldTransform_.translation_.x,worldTransform_.translation_.y, worldTransform_.translation_.z,
		worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	ImGui::End();
}