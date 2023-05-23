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
	void Initialize(Model* model, const Vector3& posion, const Vector3& velocity);


	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	// �Փ˂����m������Ăяo�����֐�
	void OnConllision();

	bool IsDead() const { return isDead_; }

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	const float radius_ = 1;

	private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	// �e�N�X�`���n���h��
	uint32_t m_textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};
