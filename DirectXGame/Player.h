#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"
#include "PlayerBullet.h"
#include <list>
#include "MathFunction.h"
#include "Sprite.h"

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

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// マウスでレティクル動かす
	/// </summary>
	void ReticleMouse();

	// テクスチャハンドル
	uint32_t m_textureHandle_ = 0u;

	Model* model_ = nullptr;

	//キーボード入力
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//ワールド座標を取得
	Vector3 GetWorldPosition();

		//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	const float radius_ = 2;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//モデル
	Model* retiModel_ = nullptr;

	//2dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	private:
	//弾
	    std::list<PlayerBullet*> bullets_;
	//PlayerBullet* bullet_ = nullptr;
	

};
