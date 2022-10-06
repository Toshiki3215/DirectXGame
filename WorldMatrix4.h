#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"

Matrix4 ScaleMatrix4(Matrix4 matWorld, Vector3 scale);

Matrix4 RotationXMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 RotationYMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 RotationZMatrix4(Matrix4 matWorld, Vector3 rotation);

Matrix4 MoveMatrix4(Matrix4 matWorld, Vector3 translation);

Vector3 transform(const Vector3& v, const Matrix4& m);

const Vector3 operator*(const Vector3& v, const Matrix4& m);

// 2項演算子オーバーロード
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
