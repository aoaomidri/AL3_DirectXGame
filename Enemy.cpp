#include "Enemy.h"
#include<assert.h>
#include"ImGuiManager.h"
#include"Player.h"

Enemy::~Enemy() { 

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	for (EnemyEffect* effect:effects_) {
		delete effect;
	}

}

void Enemy::Initialaize(Model* model, uint32_t textureHandle) {
	// �ʂ�ۃ`�F�b�N
	assert(model);
	// �����o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = textureHandle;
	
	worldTransform_.translation_ = {10, 10, 100};

	worldTransform_.Initialize();
	
	ApproathReset();

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
}

void Enemy::ApproathReset() {
	//���˃^�C�}�[������
	fireTimer = 10;
}

// �X�V
void Enemy::Update() {
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	effects_.remove_if([](EnemyEffect* effect) {
		if (effect->IsDead()) {
			delete effect;
			return true;
		}
		return false;
	});
	if (isDead_==true) {
		if (effectOn == true) {
			for (int i = 0; i < 20; i++) {
				HitEffect();
			}
		}
		effectOn = false;
	}

	if (input_->TriggerKey(DIK_LSHIFT)) {
		isDead_ = false;
		effectOn = true;
	}
	


	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		if (isDead_ == false) {
			// �ړ�
			move.z = -0.1f;
			worldTransform_.AddTransform(move);
			// ���˃^�C�}�[�J�E���g�_�E��
			fireTimer -= 1;
		}
		if (worldTransform_.translation_.z<0.0f) {
			phase_ = Phase::Leave;
		}
		
		//�w�莞�ԂɒB����
		if (fireTimer == 0){
			// �e�𔭎�
			Fire();
			//�^�C�}�[������
			fireTimer = kFireInterval;
		}
		
		break;
	case Enemy::Phase::Leave:
		if (isDead_ == false) {
			move.z = 0.3f;
			worldTransform_.AddTransform(move);
		}

		break;
	
	}	

	worldTransform_.UpdateMatrix(scale);


	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	for (EnemyEffect* Effect : effects_) {
		Effect->Update();
	}

}

// �`��
void Enemy::Draw(ViewProjection viewProjection) {
	if (isDead_ == false){
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
	
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	for (EnemyEffect* effect : effects_) {
		effect->Draw(viewProjection);
	}

}

void Enemy::Attack() { 
	if (input_->TriggerKey(DIK_RSHIFT)) {

		// �e�̑��x
		const float kBulletSpeed = -1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		//
		bullets_.push_back(newBullet);
	}

}

void Enemy::Fire() { 
	assert(player_);

		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = {0, 0, 0};
		Vector3 playerPos = player_->GetWorldPosition();
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
		//
		bullets_.push_back(newBullet);
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos(0, 0, 0);

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::HitEffect() {
	effectPos = GetWorldPosition();

	EnemyEffect* newEffect;

	newEffect = new EnemyEffect;

	newEffect->Initialize(model_, effectPos);
	//
	effects_.push_back(newEffect);

	newEffect->HitEffect();
}

