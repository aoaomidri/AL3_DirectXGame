#include"WorldTransform.h"
#include"Matrix.h"

void WorldTransform::UpdateMatrix(Vector3& scale) {
	Matrix matrix;
	//�X�P�[���A��]�A���s�ړ����������čs����v�Z����
	matWorld_ = matrix.MakaAffineMatrix(scale, rotation_, translation_);
	//�萔�o�b�t�@�ɓ]������
	TransferMatrix();
}

void WorldTransform::AddTransform(Vector3& velocity) { 
	translation_.x += velocity.x;
	translation_.y += velocity.y;
	translation_.z += velocity.z;
}