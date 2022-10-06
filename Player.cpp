#include "Player.h"
#include "TextureManager.h"
#include "WorldMatrix4.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	// 3D���f���̐���
	 model_ = Model::Create();

	 //�t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	 textureHandle_ = TextureManager::Load("player.png");

	 rotaCount = 0;

	 rotaPlayer = 0;

	 count = 0;

	 rotaTime = 0;

	 //�V���O���g���C���X�^���X���擾����
	 input_ = Input::GetInstance();
	 debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏�����
	 worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	 worldTransform_.translation_ = {0, -1, 0};

	 worldTransform_.scale_ = {0.5,0.5,0.2};

	 Matrix4 matRotZ = MathUtility::Matrix4Identity();
	 Matrix4 matRotY = MathUtility::Matrix4Identity();

	 Matrix4 matTrans = MathUtility::Matrix4Identity();

	//���_�̈ړ��x�N�g��
	 moveTarget = Vector3(0, 0, 0);

	 rotaTarget = Vector3(0, 0, 0);

	 playerPos = Vector3(0, 0, 0);

	 worldTransform_.Initialize();

}

void Player::UpDate(ViewProjection viewProjection) {
	 float rSpeed = 0;
	 float kSpeed = 0.1f;

	//���_�̈ړ�����
	 float EyeSpeed = 0.0f;
	 float sEyeSpeed = 0.0f;

	 float fSpeed = 0.02f;
	 float sSpeed = 0.02f;
	 float speed = 0.02f;

	 Matrix4 matTrans = MathUtility::Matrix4Identity();

	 Matrix4 matRotZ = MathUtility::Matrix4Identity();
	 Matrix4 matRotY = MathUtility::Matrix4Identity();

	 kSpeed = 0.1f;

	 //�f�X�t���O�̗������e���폜
	 bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	// ----- ���(��]���[�V����) -----
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
	
	// ----- ���_�ύX(���_��]) -----
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

	 //�L�����N�^�[�U������
	 Attack();

	 //�e�X�V
	 for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		 bullet->Update(worldTransform_);
	 }

	 //�J�����Ǐ]
	 viewProjection.eye.x = -rotaTarget.x * 10 + worldTransform_.translation_.x;

	 viewProjection.eye.z = -rotaTarget.z * 10 + worldTransform_.translation_.z;

	 viewProjection.target = {
	   rotaTarget.x * 10 + worldTransform_.translation_.x, 0,
	   rotaTarget.z * 10 + worldTransform_.translation_.z};

	 //�s��̍Čv�Z
	 worldTransform_.TransferMatrix();

	 //�s��̍Čv�Z
	 viewProjection.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection,textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::Attack() { 
	if (input_->PushKey(DIK_RETURN)) {

		playerPos = worldTransform_.translation_;

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(kBulletSpeed * rotaTarget.x, 0, kBulletSpeed * rotaTarget.z);

		//�e�������A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initilize(model_, playerPos,velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

	}
}

void Player::OnCollision() {

}