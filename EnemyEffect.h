#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyEffect {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	// �Փ˂�����Ăяo���֐�
	void OnCollision();

	float radius = 0.001f;

	void Charge(const Vector3& position);
	// �e�̃G�t�F�N�g
	void AttackEffect(int number);

	void HitEffect();

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	float size = 0.6f;

	Vector3 scale = {size, size, size};

	Vector3 velocity_;
	// ����
	static const int32_t kLifeTime = 20;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;

	int number_;
};
