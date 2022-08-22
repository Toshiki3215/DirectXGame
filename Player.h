#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private:
	  //ワールド変換データ
	  WorldTransform worldTransform_;

	  //モデル
	  Model* model_ = nullptr;

	  //テクスチャハンドル
	  uint32_t textureHandle_ = 0u;

	  Input* input_ = nullptr;

	  DebugText* debugText_ = nullptr;

	  float rotaCount;

	  float rotaPlayer;
	  float count;

	  float rotaTime;

	  Vector3 moveTarget;
	  Vector3 rotaTarget;

	  Player* player_ = nullptr;
};