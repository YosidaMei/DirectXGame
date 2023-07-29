#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

void Player::Initialize(Model* model,Vector3 pos) {
	// Nullポインタチェック
	assert(model);
	// モデル
	model_ = model;
	//m_textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, -4, 0};
	worldTransform_.translation_.z += pos.z;
	//viewProjection_.Initialize();
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
	//hp
	uint32_t textureHp = TextureManager::Load("hp_wahuu.png");
	//hpが減った時の移動座標
	Vector2 hpSetPos = {3000, 3000};
	//1個目
	hp1Sprite_ =
	    Sprite::Create(textureHp, Vector2(1000, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	hp1Sprite_->SetSize(Vector2(100, 100));
	// 2個目
	hp2Sprite_ =
	    Sprite::Create(textureHp, Vector2(1100, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	hp2Sprite_->SetSize(Vector2(100, 100));
	// 3個目
	hp3Sprite_ =
	    Sprite::Create(textureHp, Vector2(1200, 650), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	hp3Sprite_->SetSize(Vector2(100, 100));
	//画像
	uint32_t textureClear = TextureManager::Load("susi_ui_gameClear.png");
	uint32_t textureOver = TextureManager::Load("susi_ui_gameOver.png");
	uint32_t textureSusinum = TextureManager::Load("ui_number.png");
	uint32_t textureSousa = TextureManager::Load("ui_sousa.png");
	uint32_t textureStart = TextureManager::Load("susi_ui_tuto.png");
	uint32_t textureUiNokori = TextureManager::Load("ui_nokori.png");
	

	////ゲームオーバー
	gameoverSprite_ =
	    Sprite::Create(textureOver, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	gameoverSprite_->SetSize(Vector2(1000, 600));
	////ゲームクリア
	gameclearSprite_ =
	    Sprite::Create(textureClear, Vector2(640, 360), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	gameclearSprite_->SetSize(Vector2(1000, 600));
	////susiNumber
	for (int i = 0; i < 6;i++) {
		uiSusiNumSprite_[i] =
		    Sprite::Create(textureSusinum, Vector2(100, 400), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
		uiSusiNumSprite_[i]->SetSize(Vector2(200, 200));
	}
	////スタート
	uiStartSprite_ =
	    Sprite::Create(textureStart, Vector2(640, 150), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	uiStartSprite_->SetSize(Vector2(500, 300));
	//UI操作
	uiSousaSprite_ =
	    Sprite::Create(textureSousa, Vector2(100, 50), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	uiSousaSprite_->SetSize(Vector2(200, 100));
	//ui残り
	uiNokoriSprite_ =
	    Sprite::Create(textureUiNokori, Vector2(100, 280), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5));
	uiNokoriSprite_->SetSize(Vector2(200, 100));
}

Player::~Player() {

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
	delete hp1Sprite_;
	delete hp2Sprite_;
	delete hp3Sprite_;
	delete gameclearSprite_;
	delete gameoverSprite_;
	delete uiNokoriSprite_;
	delete uiSousaSprite_;
	delete uiStartSprite_;
	for (int i = 0; i < 6; i++) {
		delete uiSusiNumSprite_[i];
	}
}

void Player::Update(ViewProjection& viewProjection) {

	//キャラの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラの速さ
	const float kCharaSpeed = 0.1f;
	//押した方向で移動ベクトルを変更(左右)
	/*if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharaSpeed;
	}*/
	//上下
	if (input_->TriggerKey(DIK_DOWN)) {
		move.y -= 4;
	} else if (input_->TriggerKey(DIK_UP)) {
		move.y += 4;
	}
	//Z軸
	/*if (input_->PushKey(DIK_Q)) {
		move.z -= kCharaSpeed;
	} else if (input_->PushKey(DIK_E)) {
		move.z += kCharaSpeed;
	}*/

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	// ジョイスティック状態獲得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharaSpeed;
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	ReticleMouse(viewProjection);
	//キャラクター攻撃
	Attack();
	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}


	Rotate();

	//ダメージを受けた時のクールタイム
	if (isDamage == true) {
		coolTime++;
		if (coolTime >= 60) {
			isDamage = false;
			coolTime = 0;
		}
	}

	//s寿司クールタイム
	if (isSusi == true) {
		susiCoolTime++;
		if (susiCoolTime >= 10) {
			isSusi = false;
			susiCoolTime = 0;
		}
	}

	//HPの描画の変更
	if (Hp == 2) {
		hp3Sprite_->SetPosition(hpSetPos);
	}
	if (Hp == 1) {
		hp2Sprite_->SetPosition(hpSetPos);
	}
	if (Hp == 0) {
		hp1Sprite_->SetPosition(hpSetPos);
	}

	//スタートタイマー
	startUiTimer++;
	//数字
	for (int i = 0; i < 6; i++) {
		uiSusiNumSprite_[i]->SetTextureRect(Vector2(susiNum * 200, 0), Vector2(200, 200));
		uiSusiNumSprite_[i]->SetSize(Vector2(200, 200));
	}

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

	model_->Draw(worldTransform_, viewProjection);
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
		velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = Normalize(velocity);
		velocity = Scaler(kBulletSpeed, velocity);
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

	}

	//発射タイマー
	
	if (isAttack == false) {
		AttackTimer++;
		if (AttackTimer >= 50) {
			isAttack = true;
			AttackTimer = 0;
		}
	}
		XINPUT_STATE joyState;
		// ゲームパッド未接続なら何もせず抜ける
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}
	    if (isAttack == true) {
		    // Rトリガーを押していたら
		    if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			// 60ごとに発射
			// if (AttackTimer % 20 == 0) {}
			//  弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			// 弾を生成初期化
			PlayerBullet* newBullet = new PlayerBullet();
			// 弾を登録
			// bullet_ = newBullet;
			bullets_.push_back(newBullet);
			// 自機から照準オブジェクトへのベクトル
			velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
			velocity = Normalize(velocity);
			velocity = Scaler(kBulletSpeed, velocity);
			newBullet->Initialize(model_, GetWorldPosition(), velocity);
			isAttack = false;
		    }
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
	//HPが減る
	if (isDamage == false) {
		    Hp--;
		    isDamage = true;
	} else {
		    
	}

}

void Player::OnSusiConllision() {
	
	// 寿司何個付いたか
	if (isSusi == false) {
		    susiNum--;
		    radius_ += 0.5f;
		    isSusi = true;
	} else {
	}
}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 
	hp1Sprite_->Draw();
	hp2Sprite_->Draw();
	hp3Sprite_->Draw();
	uiSousaSprite_->Draw();
	uiNokoriSprite_->Draw();
	for (int i = 0; i < 6; i++) {
		    uiSusiNumSprite_[i]->Draw();
	}
	if (startUiTimer<=300) {
		    uiStartSprite_->Draw();
	} else {
		    startUiTimer = 1000;
	}
	if (Hp <= 0) {
		    gameoverSprite_->Draw();
	}
	if (susiNum <= 0) {
		    gameclearSprite_->Draw();
	}


}

void Player::ReticleMouse(ViewProjection& viewProjection) { 

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
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
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
	const float kDistanceTestObject = 40;
	Vector3 mulK2D = Scaler(kDistanceTestObject, mouseDirection);
	worldTransform3DReticle_.translation_ = Add(posNear, mulK2D);
	worldTransform3DReticle_.UpdateMatrix();

	////マウス
	//Matrix4x4 a;
	//Matrix4x4 b;
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
	////ビュープロジェクションをゲームシーンのビュープロジェクション使う
	//Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	////合成行列の逆行列を計算する
	//Matrix4x4 matInverseVPV = Inverse(matVPV);
	//a = Inverse(matVPV);
	//b = Multiply(a, matVPV);
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
	//const float kDistanceTestObject = 40;
	//Vector3 mulK2D = Scaler(kDistanceTestObject, mouseDirection);
	//worldTransform3DReticle_.translation_ = Add(posNear,mulK2D);
	//worldTransform3DReticle_.UpdateMatrix();

	/*ImGui::Begin("Player");
	
	ImGui::Text("HP:%d",Hp);
	ImGui::Text("isdame:%d", isDamage);
	ImGui::Text("cooltime:%d", coolTime);
	ImGui::Text("isSusi:%d", susiNum);
	ImGui::Text(
	    "PlayerPos x:%f y:%f z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	
	ImGui::End();*/
}
