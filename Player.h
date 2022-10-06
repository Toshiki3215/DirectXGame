#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include "PlayerBullet.h"
#include<memory>
#include<list>

/// <summary>
/// ���L����
/// </summary>
class Player {
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void UpDate(ViewProjection viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

  private:
	  //���[���h�ϊ��f�[�^
	  WorldTransform worldTransform_;

	  //�e�N�X�`���n���h��
	  uint32_t textureHandle_ = 0;

	  //���f��
	  Model* model_ = nullptr;

	  Input* input_ = nullptr;

	  DebugText* debugText_ = nullptr;

	  float rotaCount;

	  float rotaPlayer;
	  float count;

	  float rotaTime;

	  Vector3 moveTarget;
	  Vector3 rotaTarget;

	  Vector3 playerPos;

	  Player* player_ = nullptr;

	  //�e
	  std::list<std::unique_ptr<PlayerBullet>> bullets_;
};