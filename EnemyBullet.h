#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include"Matrix.h"


class EnemyBullet {
public:

	void Initialize(Model* model, Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewpprojection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	// �Փ˂�����Ăяo���֐�
	void OnCollision();

	float radius = 1.0f;

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t texturerHandle_ = 0u;

	float size = 0.5f;

	Vector3 scale = {size, size, size};

	Vector3 velocity_;
	// ����
	static const int32_t kLifeTime = 60 * 3;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;

};
