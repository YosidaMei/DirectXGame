#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"
#include "PlayerBullet.h"
#include <list>
#include "MathFunction.h"

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// デストラクタ
	/// </summary>


	// テクスチャハンドル
	uint32_t m_textureHandle_ = 0u;

	Model* model_ = nullptr;


	// ワールド変換データ
	WorldTransform worldTransform_;
};
