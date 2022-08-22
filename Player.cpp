#include "Player.h"
#include <cassert>

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale);

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation);

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation);
Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation);


void Player::Initialize(Model* model, uint32_t textureHandle_)
{
	//NULLポイントチェック
	//assert(model);

	// 3Dモデルの生成
	 model_ = Model::Create();

	 rotaCount = 0;

	 rotaPlayer = 0;

	 count = 0;

	 rotaTime = 0;

	 //シングルトンインスタンスを取得する
	 input_ = Input::GetInstance();
	 debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	 worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	 worldTransform_.translation_ = {0, 0, 0};

	 worldTransform_.scale_ = {0.5,0.5,0.2};

	 Matrix4 matRotZ = MathUtility::Matrix4Identity();
	 Matrix4 matRotY = MathUtility::Matrix4Identity();

	 Matrix4 matTrans = MathUtility::Matrix4Identity();

	//視点の移動ベクトル
	 moveTarget = Vector3(0, 0, 0);

	 rotaTarget = Vector3(0, 0, 0);

	 worldTransform_.Initialize();
}

void Player::UpDate() 
{
	 float rSpeed = 0;
	 float kSpeed = 0.1f;

	//視点の移動速さ
	 float EyeSpeed = 0.0f;
	 float sEyeSpeed = 0.0f;

	 float fSpeed = 0.02f;
	 float sSpeed = 0.02f;
	 float speed = 0.02f;

	 Matrix4 matTrans = MathUtility::Matrix4Identity();

	 Matrix4 matRotZ = MathUtility::Matrix4Identity();
	 Matrix4 matRotY = MathUtility::Matrix4Identity();

	 kSpeed = 0.1f;

	// ----- 回避(回転モーション) -----
	 if (input_->PushKey(DIK_SPACE)) {
		rotaTime = 20;
	 }

	 if (rotaTime>0) {
		rotaTime--;
		rSpeed = 3.14 / 10;
		worldTransform_.rotation_.z -= rSpeed;
		if (rotaTime <= 0) {
			rSpeed = 0;
			worldTransform_.rotation_.z = 0;
			rotaTime = 0;
		}
	 }
	
	// ----- 視点変更(視点回転) -----
	 if (input_->PushKey(DIK_LEFT)) {
		if (rotaCount == 0) {
			rSpeed = 3.14 / 2;
			rotaPlayer += rSpeed;
			rSpeed = 0;
			if (rotaPlayer >= 6.28) {
				rotaPlayer = 0;
			}
			rotaCount++;
		}

	} else if (input_->PushKey(DIK_RIGHT)) {
		if (rotaCount == 0) {
			rSpeed = 3.14 / 2;
			rotaPlayer -= rSpeed;
			rSpeed = 0;
			if (rotaPlayer <= -6.28) {
				rotaPlayer = 0;
			}
			rotaCount++;
		}
	}

	 moveTarget = {sinf(worldTransform_.rotation_.y), 0, cosf(worldTransform_.rotation_.y)};
	 rotaTarget = {sinf(rotaPlayer), 0, cosf(rotaPlayer)};

	 if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += kSpeed;
	 } else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= kSpeed;
	 }

	 if (input_->PushKey(DIK_A)) {
	    worldTransform_.translation_.x += rotaTarget.z * kSpeed;
	    worldTransform_.translation_.z += rotaTarget.x * kSpeed;
	} else if (input_->PushKey(DIK_D)) {
	    worldTransform_.translation_.x += rotaTarget.z * -kSpeed;
	    worldTransform_.translation_.z += rotaTarget.x * -kSpeed;
	}

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	worldTransform_.matWorld_ = ScaleMatrix4(worldTransform_.matWorld_, worldTransform_.scale_);

	worldTransform_.matWorld_ =
	  RotationZMatrix4(worldTransform_.matWorld_, worldTransform_.rotation_);

	worldTransform_.matWorld_ =
	  RotationYMatrix4(worldTransform_.matWorld_, worldTransform_.rotation_);

	worldTransform_.matWorld_ =
	  MoveMatrix4(worldTransform_.matWorld_, worldTransform_.translation_);

	 count++;
	 if (count == 60) {
	     rotaCount = 0;
	     count = 0;
	 }

	 //行列の再計算
	 worldTransform_.TransferMatrix();
}

void Player::Draw() 
{

}

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale) {
	Matrix4 matScale = MathUtility::Matrix4Identity();

	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;

	return matWorld *= matScale;
}

Matrix4 RotationXMatrix4(Matrix4 matWorld, Vector3 rotation) {
	Matrix4 matRotX = MathUtility::Matrix4Identity();

	matRotX.m[1][1] = cosf(rotation.x);
	matRotX.m[1][2] = sinf(rotation.x);
	matRotX.m[2][1] = -sinf(rotation.x);
	matRotX.m[2][2] = cosf(rotation.x);

	return matWorld *= matRotX;
}

Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation) {
	Matrix4 matRotY = MathUtility::Matrix4Identity();

	matRotY.m[0][0] = cosf(rotation.y);
	matRotY.m[0][2] = -sinf(rotation.y);
	matRotY.m[2][0] = sinf(rotation.y);
	matRotY.m[2][2] = cosf(rotation.y);

	return matWorld *= matRotY;
}

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation) {
	Matrix4 matRotZ = MathUtility::Matrix4Identity();

	matRotZ.m[0][0] = cosf(rotation.z);
	matRotZ.m[0][1] = sinf(rotation.z);
	matRotZ.m[1][0] = -sinf(rotation.z);
	matRotZ.m[1][1] = cosf(rotation.z);

	return matWorld *= matRotZ;
}

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;

	return matWorld *= matTrans;
}
