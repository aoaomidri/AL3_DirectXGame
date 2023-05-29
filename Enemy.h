#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Matrix.h"
#include"EnemyBullet.h"
#include"EnemyEffect.h"
#include<list>

class Player;

class Enemy {
public:
	~Enemy();

	enum class Phase {
		Approach, // �ڋ߂���
		Leave,    // ���E����
	};

	// ������
	void Initialaize(Model* model, uint32_t textureHandle);

	// �X�V
	void Update();

	// �`��
	void Draw(ViewProjection viewProjection);
	
	void Attack();

	//�e����
	void Fire();

	void ApproathReset();

	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; }
	// �Փ˂�����Ăяo���֐�
	void OnCollision() { isDead_ = true; }

	Vector3 GetWorldPosition();

	// �e���X�g���擾
	const std::list<EnemyBullet*>& GetBullets() { return bullets_;}


	float radius = 1.5f;

	// �e�̃G�t�F�N�g
	void HitEffect();

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// �L�[�{�[�h����
	Input* input_ = nullptr;
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// �s��̍쐬
	Matrix matrix;
	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy::*eFuncTable[])();

	float size = 1.0f;

	Vector3 scale = {size, size, size};

	std::list<EnemyBullet*> bullets_;

	std::list<EnemyEffect*> effects_;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
	
	Player* player_ = nullptr;
		// �f�X�t���O
	bool isDead_ = false;

	bool effectOn = true;

	Vector3 effectPos = {100, 100, -100};

	void ApproachUpdate();

	void LeaveUpdate();
};
