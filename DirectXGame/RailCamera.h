#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Matrix.h"
#include "MathFunction.h"

class RailCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform& worldTransform);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	// キーボード入力
	Input* input_ = nullptr;

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	private:

		//ワールド返還
	    WorldTransform worldTransform_;
		//ビュープロジェクション
	    ViewProjection viewProjection_;

};
