#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
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
	Vector3 playerPosition(0, 0, 15);//前にずらす量
	player_ = new Player();
	player_->Initialize(model_, textureHandle_,playerPosition);
	//敵キャラ
	
	//天球
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("tenkyuu", true);
	skydome_->Initialize(modelSkydome_);

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);
	//レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//レティクルのテクスチャ
	TextureManager::Load("syoujun.png");
}

void GameScene::Update() { 
	player_->Update(viewProjection_);
	debugCamera_->Update();
	LoadEnemyPopData();
	//敵キャラに自キャラのアドレスを渡す
	for (Enemy* enemy : enemys_) {
		enemy->SetPlayer(player_);
	}
	//UpdateEnemyPopCommands();
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	skydome_->Update();
	railCamera_->Update();
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
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	CheckAllCollisions();

	//敵の弾
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
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
	player_->DrawUI();
	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 
	//判定対象A,Bの座標
	Vector3 posA, posB; 
	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		for (Enemy* enemy : enemys_) {

			// 敵弾の座標
			posB = bullet->GetWorldPosition();
			float length = ((posB.x - posA.x) * (posB.x - posA.x)) +
			               ((posB.y - posA.y) * (posB.y - posA.y)) +
			               ((posB.z - posA.z) * (posB.z - posA.z));
			// 球と球の交差判定
			if (length <=
			    (player_->radius_ + bullet->radius_) * (player_->radius_ + bullet->radius_)) {
				// 自キャラの衝突時
				player_->OnConllision();
				// 敵弾の衝突時
				bullet->OnConllision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
		// 敵の座標
	for (Enemy* enemy : enemys_) {
		posB = enemy->GetWorldPosition();
		// 自弾と敵すべての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			posA = bullet->GetWorldPosition();

			float length = ((posB.x - posA.x) * (posB.x - posA.x)) +
			               ((posB.y - posA.y) * (posB.y - posA.y)) +
			               ((posB.z - posA.z) * (posB.z - posA.z));
			// 球と球の交差判定
			if (length <=
			    (enemy->radius_ + bullet->radius_) * (enemy->radius_ + bullet->radius_)) {
				// 敵の衝突時
				enemy->OnConllision();
				//弾の衝突時
				bullet->OnConllision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletA : playerBullets) {
		for (EnemyBullet* bulletB : enemyBullets_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { 
	enemyBullets_.push_back(enemyBullet); 
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());
	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (isWait == true) {
		waitTimer--;
		if (waitTimer <= 0) {
		//待機官僚
			isWait = false;
		}
		return;
	}
	
	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word;
	//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
		//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
		//x
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

		//y
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

		//z
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

		//敵を発生させる
			//  弾を生成初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialize(model_,Vector3(x,y,z));
			enemys_.push_back(newEnemy);
			newEnemy->SetGameScene(this);
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			//待ち時間
			int32_t waitTime = atoi(word.c_str());
			//待機開始
			isWait = true;
			waitTimer = waitTime;
			//コマンドループを抜ける
			break;
		}

	}

}
