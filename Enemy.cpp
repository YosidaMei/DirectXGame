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
	worldTransform_.translation_ = {5, 3, 20};
	
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	approachInitialize();
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}


void Enemy::Update(){
	worldTransform_.UpdateMatrix();


	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	/*
	*/

	// キャラの速さ
	const float kCharaSpeed = 0.1f;
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

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//行動切り替え
	switch (phase_) {
	case Phase::Approach:
	default:
		//移動
		worldTransform_.translation_.z -= kCharaSpeed;
		//既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < -100.0f) {
			phase_ = Phase::Leave;
		}

		//発射
		fireTimer--;
		if (fireTimer <= 0) {
			Fire();
			fireTimer = kFireInterval;
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
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = -0.2f;
	Vector3 velocity(0, 0, kBulletSpeed);
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	// 弾を生成初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録
	// bullet_ = newBullet;
	bullets_.push_back(newBullet);


}

void Enemy::approachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}