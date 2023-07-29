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
	void Initialize(Model* model,Vector3 pos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(ViewProjection& viewProjection);
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
	void ReticleMouse(ViewProjection& viewProjection);

	/// <summary>
	/// 寿司の当たり判定
	/// </summary>
	void OnSusiConllision();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	// テクスチャハンドル
	//uint32_t m_textureHandle_ = 0u;

	Model* model_ = nullptr;

	//キーボード入力
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	//ViewProjection viewProjection_;

	//ワールド座標を取得
	Vector3 GetWorldPosition();

		//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	float radius_ = 1.0f;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//モデル
	Model* retiModel_ = nullptr;

	//2dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	//hpのがぞう
	Sprite* hp1Sprite_ = nullptr;
	Sprite* hp2Sprite_ = nullptr;
	Sprite* hp3Sprite_ = nullptr;
	//UIの画像
	Sprite* uiStartSprite_ = nullptr;
	Sprite* gameoverSprite_ = nullptr;
	Sprite* gameclearSprite_ = nullptr;
	Sprite* uiSousaSprite_ = nullptr;
	Sprite* uiNokoriSprite_ = nullptr;
	Sprite* uiSusiNumSprite_[6] = {nullptr};

	int AttackTimer = 0;
	bool isAttack = false;

	int Hp = 3;
	bool isDamage = false;
	int coolTime = 0;

	bool isSusi = false;
	int susiNum = 5;
	int susiCoolTime = 0;

	Vector2 hpSetPos = {3000, 3000};

	int startUiTimer = 0;

	private:
	//弾
	    std::list<PlayerBullet*> bullets_;
	//PlayerBullet* bullet_ = nullptr;
	

};
