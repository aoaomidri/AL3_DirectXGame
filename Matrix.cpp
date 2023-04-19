#include "Matrix.h"
Matrix::Matrix(){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ScaleMatrix.m[i][j] = 0.0f;
			RotateMatrix.m[i][j] = 0.0f;
			RotateMatrixX.m[i][j] = 0.0f;
			RotateMatrixY.m[i][j] = 0.0f;
			RotateMatrixZ.m[i][j] = 0.0f;
			TranslateMatrix.m[i][j] = 0.0f;
		}
	}

};

////積
Matrix4x4 Matrix::Multiply(const Matrix4x4& mat1, const Matrix4x4& mat2) {
	Matrix4x4 result = {0.0f};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = mat1.m[i][0] * mat2.m[0][j] + mat1.m[i][1] * mat2.m[1][j] +
			                 mat1.m[i][2] * mat2.m[2][j] + mat1.m[i][3] * mat2.m[3][j];
		}
	}
	return result;
}


Matrix4x4 Matrix::MakaScaleMatrix(const Vector3& scale){ 
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 Matrix::MakeRotateMatrixX(const Vector3& rot) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = cosf(rot.x);
	result.m[1][2] = sinf(rot.x);
	result.m[2][1] = -sinf(rot.x);
	result.m[2][2] = cosf(rot.x);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 Matrix::MakeRotateMatrixY(const Vector3& rot) {
	Matrix4x4 result{};
	result.m[1][1] = 1.0f;
	result.m[0][0] = cosf(rot.x);
	result.m[0][2] = sinf(rot.x);
	result.m[2][0] = -sinf(rot.x);
	result.m[2][2] = cosf(rot.x);
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 Matrix::MakeRotateMatrixZ(const Vector3& rot) {
	Matrix4x4 result{};
	result.m[2][2] = 1.0f;
	result.m[0][0] = cosf(rot.x);
	result.m[0][1] = sinf(rot.x);
	result.m[1][0] = -sinf(rot.x);
	result.m[1][1] = cosf(rot.x);
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 Matrix::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;

}

Matrix4x4
    Matrix::MakaAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	Matrix4x4 result{};

	//スケーリング行列の作成
	ScaleMatrix = MakaScaleMatrix(scale);
	//X,Y,Z軸の回転行列の作成
	RotateMatrixX = MakeRotateMatrixX(rot);
	RotateMatrixY = MakeRotateMatrixY(rot);
	RotateMatrixZ = MakeRotateMatrixZ(rot);
	//回転行列の結合
	RotateMatrix = Multiply(RotateMatrixX, Multiply(RotateMatrixY, RotateMatrixZ));
	//平行移動行列の作成
	TranslateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(ScaleMatrix, Multiply(RotateMatrix, TranslateMatrix));

	return result;



}