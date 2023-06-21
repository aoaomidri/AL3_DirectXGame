#include"WorldTransform.h"
#include"Matrix.h"

void WorldTransform::UpdateMatrix(Vector3& scale) {
	Matrix matrix;
	//スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = matrix.MakaAffineMatrix(scale, rotation_, translation_);
	//親があれば親のワールド行列をかける
	if (parent_) {
		matWorld_ = matrix.Multiply(matWorld_, parent_->matWorld_);
	}
	//定数バッファに転送する
	TransferMatrix();
}

void WorldTransform::AddTransform(Vector3& velocity) { 
	translation_.x += velocity.x;
	translation_.y += velocity.y;
	translation_.z += velocity.z;
}

void WorldTransform::CalculationMatrix(Vector3& scale, Vector3& rotate, Vector3& translation) {
	Matrix matrix;
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = matrix.MakaAffineMatrix(scale, rotate, translation);
}

