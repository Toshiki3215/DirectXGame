#include "Enemy.h"
#include "WorldMatrix4.h"
#include <cassert>

void Enemy::Initilize(Model* model) {
	model_ = Model::Create();

	//テクスチャ読み込み
	textureHandle2_ = TextureManager::Load("enemy.png");
	textureHandle3_ = TextureManager::Load("mario.jpg");

	//ワールドトランスフォームの初期化
	worldTransformEnemy_.rotation_ = {0.0f, 0.0f, 0.0f};

	worldTransformEnemy_.translation_ = {0.0f, 0.0f, 10.0f};

	worldTransformEnemy_.scale_ = {1.0,1.0,1.0};

	worldTransformEnemy_.Initialize();

	const float kEnemySpeed = 0.005f;
	const float kEnemySpeed2 = 0.05f;

	Vector3 velocity(0, 0, kEnemySpeed);
	Vector3 velocity2(0, 0, kEnemySpeed2);
	Vector3 velocity3(0, 0, 0);
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	velocity2_ = velocity2;

	velocity3_ = velocity3;
}

void Enemy::Update(Vector3 velocity) {

	//座標を移動させる(1フレーム分の移動量を足す)
	//worldTransformEnemy_.translation_ -= velocity_;

	switch (phase_) { 
	case Phase::Approch:
	default:
		//移動(ベクトル加算)
		worldTransformEnemy_.translation_ -= velocity_;
		textureHandle_ = textureHandle2_;
		//規定の位置に到着したら離脱
		if (worldTransformEnemy_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		//移動(ベクトルを加算)
		worldTransformEnemy_.translation_ -= velocity2_;
		break;

	case Phase::Death:
		worldTransformEnemy_.translation_ = velocity3_;
		break;
	}


	worldTransformEnemy_.matWorld_ = MathUtility::Matrix4Identity();

	worldTransformEnemy_.matWorld_ =
	  ScaleMatrix4(worldTransformEnemy_.matWorld_, worldTransformEnemy_.scale_);

	worldTransformEnemy_.matWorld_ =
	  RotationZMatrix4(worldTransformEnemy_.matWorld_, worldTransformEnemy_.rotation_);

	worldTransformEnemy_.matWorld_ =
	  RotationYMatrix4(worldTransformEnemy_.matWorld_, worldTransformEnemy_.rotation_);

	worldTransformEnemy_.matWorld_ =
	  MoveMatrix4(worldTransformEnemy_.matWorld_, worldTransformEnemy_.translation_);

	//行列の再計算
	worldTransformEnemy_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransformEnemy_, viewProjection, textureHandle_);
}

void Enemy::OnCollision() { 
	phase_ = Phase::Death; 
	textureHandle_ = textureHandle3_;
}

Vector3 Enemy::GetWorldPos() { 
	worldTransformEnemyPos_.x = worldTransformEnemy_.matWorld_.m[3][0]; 
	worldTransformEnemyPos_.y = worldTransformEnemy_.matWorld_.m[3][1]; 
	worldTransformEnemyPos_.z = worldTransformEnemy_.matWorld_.m[3][2]; 

	return worldTransformEnemyPos_;
}