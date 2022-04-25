#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	model_ = Model::Create();

	worldTransform_.scale_ = {1.0f, 1.0f, 0.5f};

	worldTransform_.rotation_ = {0.0f,0.0f, 0.0f};

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	viewProjection_.eye = {0, 0, -10};

	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void GameScene::Update()
{ 
	XMFLOAT3 target = {0, 0, 0};

	//移動速さ
	const float kSpeed = 0.1f;

	//回転角度(2度ずつ(180/90))
	const float rSpeed = XM_PI / 90.0f;

	//A,Dで回転
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += rSpeed;
	} else if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += -rSpeed;
	}

	//単位ベクトルのターゲット座標
	target = {sinf(worldTransform_.rotation_.y), 0, cosf(worldTransform_.rotation_.y)};

	//W,Sで移動
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.x += target.x * kSpeed;
		worldTransform_.translation_.z += target.z * kSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.x += target.x * -kSpeed;
		worldTransform_.translation_.z += target.z * -kSpeed;
	}

	worldTransform_.UpdateMatrix();

	std::string strDebug_1 = std::string("translation:(") +
	                         std::to_string(worldTransform_.translation_.x) + std::string(",") +
	                         std::to_string(worldTransform_.translation_.y) + std::string(",") +
	                         std::to_string(worldTransform_.translation_.z) + std::string(")");

	std::string strDebug_2 = std::string("rotation:(") +
	                         std::to_string(worldTransform_.rotation_.x) + std::string(",") +
	                         std::to_string(worldTransform_.rotation_.y) + std::string(",") +
	                         std::to_string(worldTransform_.rotation_.z) + std::string(")");

	debugText_->Print(strDebug_1, 50, 50, 1.0f);

	debugText_->Print(strDebug_2, 50, 75, 1.0f);

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
