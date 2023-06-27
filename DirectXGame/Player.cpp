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
	viewProjection_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクルモデル
	retiModel_ = Model::Create();
	uint32_t textureReticle = TextureManager::Load("syoujun.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle, Vector2(400,400),
	    Vector4(1,1,1,1), Vector2(0.5, 0.5));
	sprite2DReticle_->SetSize(Vector2(100, 100));
}

Player::~Player() {

	//delete bullet_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;

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

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	// ジョイスティック状態獲得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharaSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharaSpeed;
	}

	//座標移動(ベクトルの加算)
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

	ReticleMouse();

	

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
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
	//弾を生成初期化
		PlayerBullet* newBullet = new PlayerBullet();
		//弾を登録
		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
	//自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		velocity = Normalize(velocity);
		velocity = Scaler(kBulletSpeed, velocity);
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

	}

	XINPUT_STATE joyState;
	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	//Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 弾を生成初期化
		PlayerBullet* newBullet = new PlayerBullet();
		// 弾を登録
		// bullet_ = newBullet;
		bullets_.push_back(newBullet);
		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		velocity = Normalize(velocity);
		velocity = Scaler(kBulletSpeed, velocity);
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
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

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 
}

void Player::ReticleMouse() { 

	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	////ゲームパッドの状態を得る変数
	XINPUT_STATE joyStateSP;
	// ジョイスティック状態獲得
	if (Input::GetInstance()->GetJoystickState(0, joyStateSP)) {
		spritePosition.x += (float)joyStateSP.Gamepad.sThumbRX / SHRT_MAX * 7.0f;
		spritePosition.y -= (float)joyStateSP.Gamepad.sThumbRY / SHRT_MAX * 7.0f;
		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}
	// ビューポート
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
	    Multiply(Multiply(viewProjection_.matView, viewProjection_.matProjection), matViewport);
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	// スクリーン座標
	Vector3 posNear =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);
	// スクリーン座標からワールド座標へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);
	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100;
	Vector3 mulK2D = Scaler(kDistanceTestObject, mouseDirection);
	worldTransform3DReticle_.translation_ = Add(posNear, mulK2D);
	worldTransform3DReticle_.UpdateMatrix();

	//POINT mousePosition; 
	////マウス座標を取得(スクリーン)
	//GetCursorPos(&mousePosition);
	////クライアントエリア座標に変換する(スクリーン座標)
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);
	////マウス座標を２ⅾレティクルのスプライトに代入
	//sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));
	//// ビューポート
	//Matrix4x4 matViewport =
	//    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	////ビュープロジェクションビューポート合成行列
	//Matrix4x4 matVPV = Multiply(Multiply(viewProjection_.matView, viewProjection_.matProjection), matViewport);
	////合成行列の逆行列を計算する
	//Matrix4x4 matInverseVPV = Inverse(matVPV);
	////スクリーン座標
	//Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	//Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);
	////スクリーン座標からワールド座標へ
	//posNear = Transform(posNear, matInverseVPV);
	//posFar = Transform(posFar, matInverseVPV);
	////マウスレイの方向
	//Vector3 mouseDirection = Subtract(posFar, posNear);
	//mouseDirection = Normalize(mouseDirection);
	////カメラから照準オブジェクトの距離
	//const float kDistanceTestObject = 120;
	//Vector3 mulK2D = Scaler(kDistanceTestObject, mouseDirection);
	//worldTransform3DReticle_.translation_ = Add(posNear,mulK2D);
	//worldTransform3DReticle_.UpdateMatrix();

	//ImGui::Begin("Player");
	//ImGui::Text("2dReticle:(%d,%d)", mousePosition.x, mousePosition.y);
	//ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y,posNear.z);
	//ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y,posFar.z);
	//ImGui::Text(
	//    "3dReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	//    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);	
	//ImGui::End();
}
