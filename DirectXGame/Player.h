#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"
#include "PlayerBullet.h"
#include <list>
#include "MathFunction.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	void Rotate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//衝突を検知したら呼び出される関数
	void OnConllision();

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	void SetParent(const WorldTransform* parent);

	// テクスチャハンドル
	uint32_t m_textureHandle_ = 0u;

	Model* model_ = nullptr;

	//キーボード入力
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	//ワールド座標を取得
	Vector3 GetWorldPosition();

		//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	const float radius_ = 2;

	private:
	//弾
	    std::list<PlayerBullet*> bullets_;
	//PlayerBullet* bullet_ = nullptr;
	

};
