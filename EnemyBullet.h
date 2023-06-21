#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include"Matrix.h"

class Player;

class EnemyBullet {
public:

	void Initialize(Model* model, Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewpprojection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }

	// �Փ˂�����Ăяo���֐�
	void OnCollision();

	float radius = 1.0f;
	//1�t���[���ł̕ۊǊ���
	const float t = 1.0f;

	const float kBulletSpeed = 0.9f;

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t texturerHandle_ = 0u;

	float size = 0.1f;

	Vector3 scale = {0.5f, 0.5f, 3.0f};

	Vector3 velocity_;
	// ����
	static const int32_t kLifeTime = 60 * 3;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;

	Player* player_ = nullptr;

	// �s��̍쐬
	Matrix matrix_;

	Vector3 vec;

	Matrix4x4 minusMatrix{0};

	Vector3 minusVelocity{0, 0, 0};

	Vector3 velocityZ{0, 0, 0};

	Vector3 playerPos = {};

	Vector3 toPlayer = {};
};
