#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "Player.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, Vector3 position) {
	// Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	//m_textureHandle_ = TextureManager::Load("AL3wadoru.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	approachInitialize();
	
}

Enemy::~Enemy() {
	
}


void Enemy::Update(){

	if (isPlayer) {
		//worldTransform_.translation_.z -= 1;
		/*Matrix4x4 keepRotate = worldTransform_.matWorld_;
		worldTransform_.matWorld_ = Add(player_->worldTransform_.matWorld_, keepRotate);*/
	}
	else {

		// キャラの速さ
		const float kCharaSpeed = 0.05f;
		const float kLeaveSpeed = 0.2f;

		// 行動切り替え
		switch (phase_) {
		case Phase::Approach:
		default:
			// 移動
			worldTransform_.translation_.z -= kCharaSpeed;
			// 既定の位置に到達したら離脱
			if (worldTransform_.translation_.z < -80.0f) {
				phase_ = Phase::Leave;
			}

			if (worldTransform_.translation_.z < 12.5f) {
			} else {
				// 発射
				fireTimer--;
				if (fireTimer <= 0) {
					Fire();
					fireTimer = kFireInterval;
				}
			}
			
			break;
		case Phase::Leave:
			// 移動
			worldTransform_.translation_.x -= kLeaveSpeed;
			worldTransform_.translation_.y += kLeaveSpeed;

			break;
		}

	}

	worldTransform_.UpdateMatrix();

	//プレイヤーに当たった時にくっつける
	//プレイヤーと親子関係を結ぶ
	//くっついた際にプレイヤーと同じ向きに向くので、対策として敵の回転（rotation）を新しく作った回転行列に記録しプレイヤーの回転と足す

}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);
	
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 0.25f;
	
	Vector3 player_worldPos = player_->GetWorldPosition();
	Vector3 enemy_worldPos = GetWorldPosition();
	Vector3 newVector = {
	    player_worldPos.x - enemy_worldPos.x,
	    player_worldPos.y - enemy_worldPos.y,
	    player_worldPos.z - enemy_worldPos.z
	};

	Normalize(newVector);
	newVector.x *= kBulletSpeed;
	newVector.y *= kBulletSpeed;
	newVector.z *= kBulletSpeed;

	Vector3 velocity(newVector);
	// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	
	// 弾を生成初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, GetWorldPosition(), velocity);
	// 弾を登録
	//enemyBullets_.push_back(newBullet);

	gameScene_->AddEnemyBullet(newBullet);

}

void Enemy::approachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::OnConllision() { 
	// デスフラグ
	isDead_ = true;
}

void Enemy::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent;
}