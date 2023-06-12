#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Matrix.h"
#include "MathFunction.h"

class RailCamera {

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WorldTransform& worldTransform);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	// �L�[�{�[�h����
	Input* input_ = nullptr;

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	private:

		//���[���h�Ԋ�
	    WorldTransform worldTransform_;
		//�r���[�v���W�F�N�V����
	    ViewProjection viewProjection_;

};
