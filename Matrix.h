#pragma once

#include "Vector3.h"
#include"Matrix4x4.h"
#include<math.h>

class Matrix {
public:
	Matrix();


private:
	Matrix4x4 ScaleMatrix;
	Matrix4x4 RotateMatrix;
	Matrix4x4 RotateMatrixX;
	Matrix4x4 RotateMatrixY;
	Matrix4x4 RotateMatrixZ;

	Matrix4x4 TranslateMatrix;

public:
	Matrix4x4 MakaAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
	Matrix4x4 MakaScaleMatrix(const Vector3& scale);

	Matrix4x4 MakeRotateMatrixX(const Vector3& rot);
	Matrix4x4 MakeRotateMatrixY(const Vector3& rot);
	Matrix4x4 MakeRotateMatrixZ(const Vector3& rot);

	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	////êœ
	Matrix4x4 Multiply(const Matrix4x4& mat1, const Matrix4x4& mat2);
};
