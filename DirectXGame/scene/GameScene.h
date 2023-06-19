#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();
	
	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	// ワールド、ビュー
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;
	//敵
	//Enemy* enemy_ = nullptr;
	// 敵待機フラグ
	bool isWait = false;
	// 大気タイマー
	int waitTimer = 0;
	
	//天球
	Skydome* skydome_ = nullptr; 
	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets_; }
	//敵リストを取得
	const std::list<Enemy*>& GetEnemys() { return enemys_; }

	std::list<EnemyBullet*> enemyBullets_;
	std::list<Enemy*> enemys_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;
	

};
