#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"Matrix.h"
#include"EnemyEffect.h"
#include<list>

class Player;

class GameScene;

class Enemy {
public:
	~Enemy();

	enum class Phase {
		Approach, // �ڋ߂���
		Leave,    // ���E����
	};

	// ������
	void Initialaize(Model* model, uint32_t textureHandle, Vector3& position);

	// �X�V
	void Update();

	// �`��
	void Draw(ViewProjection viewProjection);
	
	//�e����
	void Fire();

	void ApproathReset();

	// �e�̃G�t�F�N�g
	void HitEffect();

	static const int kFireInterval = 60;
	//���񂾂��ǂ���
	bool IsDead() const { return isDead_; }
	
	// �Փ˂�����Ăяo���֐�
	void OnCollision();

	Vector3 GetWorldPosition();

	float radius = 1.5f;

	//�Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

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
	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	float size = 1.0f;

	Vector3 scale = {size, size, size};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//���˃^�C�}�[
	int32_t fireTimer = 0;

	int32_t leaveTime = 600;
	
	Player* player_ = nullptr;
		// �f�X�t���O
	bool isDead_ = false;

	bool effectOn = true;

	Vector3 effectPos = {100, 100, -100};

	void ApproachUpdate();

	void LeaveUpdate();
};
