#include "Skydome.h"
#include "WorldMatrix4.h"
#include <cassert>

void Skydome::Initialize(Model* model) { 
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransformDome_.rotation_ = {0.0f, 0.0f, 0.0f};

	worldTransformDome_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransformDome_.scale_ = {10.0, 10.0, 10.0};

	worldTransformDome_.Initialize();

}

void Skydome::Update() {

	worldTransformDome_.matWorld_ = MathUtility::Matrix4Identity();

	worldTransformDome_.matWorld_ =
	  ScaleMatrix4(worldTransformDome_.matWorld_, worldTransformDome_.scale_);

	worldTransformDome_.matWorld_ =
	  RotationZMatrix4(worldTransformDome_.matWorld_, worldTransformDome_.rotation_);

	worldTransformDome_.matWorld_ =
	  RotationYMatrix4(worldTransformDome_.matWorld_, worldTransformDome_.rotation_);

	worldTransformDome_.matWorld_ =
	  MoveMatrix4(worldTransformDome_.matWorld_, worldTransformDome_.translation_);

	//行列の再計算
	worldTransformDome_.TransferMatrix();

}

void Skydome::Draw(const ViewProjection& viewProjection) {
	//3Dモデル描画
	model_->Draw(worldTransformDome_, viewProjection);
}