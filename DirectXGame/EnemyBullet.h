#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Matrix.h"

class EnemyBullet {
public:
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }

	// �Փ˂����m������Ăяo�����֐�
	void OnConllision();

	Vector3 GetWorldPosition();

	const float radius_ = 2;

	private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	// �e�N�X�`���n���h��
	uint32_t m_textureHandle_ = 0u;
	// ���x
	Vector3 velocity_;
	// ����
	static const int32_t kLifeTime = 60 * 5;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;
};
