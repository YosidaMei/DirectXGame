#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("AL3kabi.png");
	// 3Dモデル
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.farZ = 100;
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	//敵キャラ
	enemy_ = new Enemy();
	enemy_->Initialize(model_);
	//天球
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("tenkyuu", true);
	skydome_->Initialize(modelSkydome_);

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

}

void GameScene::Update() { 
	player_->Update();
	debugCamera_->Update();
	enemy_->Update();
	skydome_->Update();
#ifdef DEBUG
#endif // DEBUG
	//F押すとデバッグカメラ
	if (input_->TriggerKey(DIK_F)) {
		if (isDebugCameraActive_ != true) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	player_->Rotate();

	CheckAllCollisions();

}


void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	//2D
	

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//3Dモデル描画
	// 自キャラの描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 
	//判定対象A,Bの座標
	Vector3 posA, posB; 
	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets =enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	
	//敵弾の座標
		posB = bullet->GetWorldPosition();
		float length = ((posB.x - posA.x) * (posB.x - posA.x)) +
		               ((posB.y - posA.y) * (posB.y - posA.y)) +
		               ((posB.z - posA.z) * (posB.z - posA.z));
		//球と球の交差判定
		if (length <= (player_->radius_ + bullet->radius_) * (player_->radius_ + bullet->radius_)) {
			//自キャラの衝突時
			player_->OnConllision();
			//敵弾の衝突時
			bullet->OnConllision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
		// 敵の座標
		posB = enemy_->GetWorldPosition();

	// 自弾と敵すべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		posA = bullet->GetWorldPosition();

		float length = ((posB.x - posA.x) * (posB.x - posA.x)) +
		               ((posB.y - posA.y) * (posB.y - posA.y)) +
		               ((posB.z - posA.z) * (posB.z - posA.z));
		// 球と球の交差判定
		if (length <= (player_->radius_ + bullet->radius_) * (player_->radius_ + bullet->radius_)) {
			// 自キャラの衝突時
			player_->OnConllision();
			// 敵弾の衝突時
			bullet->OnConllision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets) {
			posA = bulletA->GetWorldPosition();
			posB = bulletB->GetWorldPosition();

			float length = ((posB.x - posA.x) * (posB.x - posA.x)) +
			               ((posB.y - posA.y) * (posB.y - posA.y)) +
			               ((posB.z - posA.z) * (posB.z - posA.z));
			// 球と球の交差判定
			if (length <=
			    (bulletB->radius_ + bulletA->radius_) * (bulletB->radius_ + bulletA->radius_)) {
				// 自キャラの衝突時
				bulletB->OnConllision();
				// 敵弾の衝突時
				bulletA->OnConllision();
			}
		}
	}

#pragma endregion
}
