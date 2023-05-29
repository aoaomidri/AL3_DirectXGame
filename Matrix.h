#pragma once

#include "Vector3.h"
#include"Matrix4x4.h"
#include<iostream>
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

	////Ï
	Matrix4x4 Multiply(const Matrix4x4& mat1, const Matrix4x4& mat2);

	//xNgÏ·
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	////tsñ
	Matrix4x4 Inverce(const Matrix4x4& mat);

	Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
	// xNgÌàÏ©çcosÆðßé
	float Dot(const Vector3& v1, const Vector3& v2);
	//³K»
	Vector3 Normalize(const Vector3& v);

};
