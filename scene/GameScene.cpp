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
	XMFLOAT3 move = {0, 0, 0};

	const float kSpeed = 0.2f;

	if (input_->PushKey(DIK_W))
	{
		move = {0, 0, kSpeed};
	} else if (input_->PushKey(DIK_S))
	{
		move = {0, 0, -kSpeed};
	}

	XMFLOAT3 move2 = {0, 0, 0};

	const float kTSpeed = XM_PI / 90.0f;

	if (input_->PushKey(DIK_D)) {
		move2 = {0,kTSpeed,0};
	} else if (input_->PushKey(DIK_A)) {
		move2 = {0,-kTSpeed,0};
	}

	worldTransform_.translation_.z += move.z;
	worldTransform_.rotation_.y += move2.y;

	//viewProjection_.eye.x += move2.y;
	//viewProjection_.eye.y -= move2.y;
	//viewProjection_.eye.z += move.z;

	worldTransform_.UpdateMatrix();
	//viewProjection_.UpdateMatrix();

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
