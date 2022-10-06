#include "Player.h"
#include "TextureManager.h"
#include "WorldMatrix4.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	// 3Dモデルの生成
	 model_ = Model::Create();

	 //ファイル名を指定してテクスチャを読み込む
	 textureHandle_ = TextureManager::Load("player.png");

	 rotaCount = 0;

	 rotaPlayer = 0;

	 count = 0;

	 rotaTime = 0;

	 //シングルトンインスタンスを取得する
	 input_ = Input::GetInstance();
	 debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	 worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	 worldTransform_.translation_ = {0, -1, 0};

	 worldTransform_.scale_ = {0.5,0.5,0.2};

	 Matrix4 matRotZ = MathUtility::Matrix4Identity();
	 Matrix4 matRotY = MathUtility::Matrix4Identity();

	 Matrix4 matTrans = MathUtility::Matrix4Identity();

	//視点の移動ベクトル
	 moveTarget = Vector3(0, 0, 0);

	 rotaTarget = Vector3(0, 0, 0);

	 playerPos = Vector3(0, 0, 0);

	 worldTransform_.Initialize();

}

void Player::UpDate(ViewProjection viewProjection) {
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

	 //デスフラグの立った弾を削除
	 bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

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

	 rotaTarget = {sinf(rotaPlayer), 0, cosf(rotaPlayer)};

	 if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += kSpeed;
	 } else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= kSpeed;
	 }

	 if (input_->PushKey(DIK_A)) {
	    worldTransform_.translation_.x += rotaTarget.z * -kSpeed;
	    worldTransform_.translation_.z += rotaTarget.x * kSpeed;
	} else if (input_->PushKey(DIK_D)) {
	    worldTransform_.translation_.x += rotaTarget.z * kSpeed;
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

	 //キャラクター攻撃処理
	 Attack();

	 //弾更新
	 for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		 bullet->Update(worldTransform_);
	 }

	 //カメラ追従
	 viewProjection.eye.x = -rotaTarget.x * 10 + worldTransform_.translation_.x;

	 viewProjection.eye.z = -rotaTarget.z * 10 + worldTransform_.translation_.z;

	 viewProjection.target = {
	   rotaTarget.x * 10 + worldTransform_.translation_.x, 0,
	   rotaTarget.z * 10 + worldTransform_.translation_.z};

	 //行列の再計算
	 worldTransform_.TransferMatrix();

	 //行列の再計算
	 viewProjection.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection,textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::Attack() { 
	if (input_->PushKey(DIK_RETURN)) {

		playerPos = worldTransform_.translation_;

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(kBulletSpeed * rotaTarget.x, 0, kBulletSpeed * rotaTarget.z);

		//弾生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initilize(model_, playerPos,velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

	}
}

void Player::OnCollision() {

}