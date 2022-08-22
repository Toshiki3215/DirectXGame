#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

/// <summary>
/// ���L����
/// </summary>
class Player {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void UpDate();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

  private:
	  //���[���h�ϊ��f�[�^
	  WorldTransform worldTransform_;

	  //���f��
	  Model* model_ = nullptr;

	  //�e�N�X�`���n���h��
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