#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 pos) {
	// Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	m_textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.z += pos.z;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクルモデル
	retiModel_ = Model::Create();
	kariTextureHandle_ = TextureManager::Load("white1x1.png");

}

Player::~Player() {

	//delete bullet_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Update() {

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
	//Z軸
	if (input_->PushKey(DIK_Q)) {
		move.z -= kCharaSpeed;
	} else if (input_->PushKey(DIK_E)) {
		move.z += kCharaSpeed;
	}

	//座標移動(ベクトルの加算)
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//移動限界の座標
	const float kMoveLimitX = 25;
	const float kMoveLimitY = 15;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.UpdateMatrix();

	//キャラクター攻撃
	Attack();
	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//デバッグ
	ImGui::Begin("PlayerDebug");
	ImGui::Text(
	    "PlayerPos x:%f y:%f z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//自機から3Dレティクルの距離
	const float kDistancePlayerTo3DReticle = 20.0f;
	//自機から3dレティクルへのオフセット
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = VecMatMultiply(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Normalize(offset);
	offset = Scaler(kDistancePlayerTo3DReticle, offset);
	//3dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_, offset);
	worldTransform3DReticle_.UpdateMatrix();
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
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	//3ⅾレティクルを描画
	retiModel_->Draw(worldTransform3DReticle_, viewProjection, kariTextureHandle_);
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	//弾を生成初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//弾を登録
		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
	//自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		velocity = Normalize(velocity);
		velocity = Scaler(kBulletSpeed, velocity);

	}

}

Vector3 Player::GetWorldPosition(){
	// ワールド座標を入れる
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;

}


void Player::OnConllision() {

}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

