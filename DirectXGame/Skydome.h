#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Skydome {

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

	private:
		//ワールド返還データ
	    WorldTransform worldTransform_;
	    Model* model_ = nullptr;
};
