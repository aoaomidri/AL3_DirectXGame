#include "BaseField.h"
#include<assert.h>
void BaseField::Initialize(const std::vector<Model*>& models) {
	//ヌルぽチェック
	for (Model* model : models){
		assert(model);
	}
	//モデルのコピー
	models_ = models;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

}

void BaseField::Update() {}

void BaseField::Draw(const ViewProjection& viewProjection) {
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}
