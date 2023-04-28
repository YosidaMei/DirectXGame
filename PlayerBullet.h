#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"

class PlayerBullet {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, const Vector3& posion);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);


	private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	// �e�N�X�`���n���h��
	uint32_t m_textureHandle_ = 0u;
};
