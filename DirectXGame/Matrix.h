#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

class Matrix {};
Matrix4x4 MakeTranslateMatrix(Vector3 translate);
Matrix4x4 MakeScaleMatrix(Vector3 scale);
Vector3 Transform(Vector3 vector, Matrix4x4 matrix);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2);
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 theta, Vector3 translate);
Matrix4x4 Inverse(Matrix4x4 matrix);
Vector3 Scaler(float scalar, Vector3& v);
Vector3 VecMatMultiply(Vector3 vector, Matrix4x4 matrix);
Vector3 Subtract(Vector3& v1, Vector3& v2);
Vector3 Add(Vector3& v1, Vector3& v2);