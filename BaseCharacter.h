#pragma once
#include<Model.h>
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"MyMatrix.h"
#include"MyVector.h"
class BaseCharacter {
protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;

public:
	//初期化
	virtual void Initialize(const std::vector<Model*>& models);

	//更新
	virtual void Update();

	//描画
	virtual void Draw(const ViewProjection& viewProjection);

	//ワールド変換データを取得
	const WorldTransform& GetWorldTransform() { return worldTransform_;}
};
