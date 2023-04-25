#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	m_textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}

void Player::Update() {
	worldTransform_.TransferMatrix();

	//キャラの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラの速さ
	const float kCharaSpeed = 0.2f;
	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharaSpeed;
	}
	//上下
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharaSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharaSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	//移動限界の座標
	const float kMoveLimitX = 25;
	const float kMoveLimitY = 15;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//デバッグ
	ImGui::Begin("PlayerDebug");
	ImGui::Text(
	    "Pos x:%f y:%f z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();

}

void Player::Rotate() {
	// 回転の速さ
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, m_textureHandle_);

}