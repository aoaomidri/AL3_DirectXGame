#include "Enemy.h"
#include<assert.h>
#include"ImGuiManager.h"
#include"Player.h"
#include"GameScene.h"

Enemy::~Enemy() { 

	
}

void Enemy::Initialaize(Model* model, uint32_t textureHandle, Vector3& position) {
	// �ʂ�ۃ`�F�b�N
	assert(model);
	// �����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = textureHandle;
	
	worldTransform_.translation_ = position;

	worldTransform_.Initialize();
	
	ApproathReset();	

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
}

void (Enemy::*Enemy::eFuncTable[])() = {
	&Enemy::ApproachUpdate,
	&Enemy::LeaveUpdate
};

void Enemy::ApproathReset() {
	//���˃^�C�}�[������
	fireTimer = 30;
}

// �X�V
void Enemy::Update() {
	
	if (input_->TriggerKey(DIK_LSHIFT)) {
		isDead_ = false;
		effectOn = true;
	}
	
	if (leaveTime < 0){
		phase_ = Phase::Leave;
	}

	(this->*eFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix(scale);

	

}

// �`��
void Enemy::Draw(ViewProjection viewProjection) {
	if (isDead_ == false){
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}	

}

//void Enemy::Attack() { 
//	if (input_->TriggerKey(DIK_RSHIFT)) {
//
//		// �e�̑��x
//		const float kBulletSpeed = -1.0f;
//		Vector3 velocity(0, 0, kBulletSpeed);
//
//		EnemyBullet* newBullet = new EnemyBullet();
//		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
//		//
//		bullets_.push_back(newBullet);
//	}
//
//}

void Enemy::Fire() { 
	assert(player_);

		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = {0, 0, 0};
		Vector3 playerPos = player_->GetPlayerWorldPosition();
	    Vector3 enemyPos = GetWorldPosition();
	    Vector3 vector = {
	        playerPos.x - enemyPos.x,
	        playerPos.y - enemyPos.y,
	        playerPos.z - enemyPos.z
	    };
	    float bulletNorm =
	        sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

		if (bulletNorm != 0.0f) {

			velocity = {
		    (vector.x / bulletNorm) * kBulletSpeed, 
			(vector.y / bulletNorm) * kBulletSpeed,
		    (vector.z / bulletNorm) * kBulletSpeed};
	    }	

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	    newBullet->SetPlayer(player_);
		//���˂����e�����X�g�ɓo�^����
	    gameScene_->AddEnemyBullet(newBullet);
		//bullets_.push_back(newBullet);
}
void Enemy::OnCollision() { 
	for (int i = 0; i < 20; i++) {
		    HitEffect();
	}
	isDead_ = true; 
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::HitEffect() {
	effectPos = GetWorldPosition();

	EnemyEffect* newEffect;

	newEffect = new EnemyEffect;

	newEffect->Initialize(model_, effectPos);
	//
	gameScene_->AddEnemyEffect(newEffect);

	newEffect->HitEffect();
}

void Enemy::ApproachUpdate() {
	if (isDead_ == false) {
		    // �ړ�
		    //move.z = -0.03f;
		    worldTransform_.AddTransform(move);
		    // ���˃^�C�}�[�J�E���g�_�E��
		    //leaveTime -= 1;
		    fireTimer -= 1;
	}
	

	// �w�莞�ԂɒB����
	if (fireTimer == 0) {
		    // �e�𔭎�
		    Fire();
		    // �^�C�}�[������
		    fireTimer = kFireInterval;
		    
	}

}

void Enemy::LeaveUpdate() {
	if (isDead_ == false) {
		    move = {-0.4f, 0.4f, -0.6f};

		    worldTransform_.AddTransform(move);
		    leaveTime = 600;
	}
}
