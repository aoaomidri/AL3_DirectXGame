#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Matrix.h"

class RailCamera {
public:
	RailCamera();
	~RailCamera();

private:
	//ワ－ルド変換データ	
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;	

	Matrix* matrix = nullptr;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	float size = 1.0f;

	Vector3 scale = {size, size, size};

public:
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldMatrix() { return worldTransform_; }

	void Initialize(Matrix4x4& worldPosition, Vector3& rotate);

	void Update();
};
