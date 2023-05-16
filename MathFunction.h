#pragma once
#include "Matrix.h"
#include "Vector3.h"
#define _USE_MATH_DEFINES
#include <cmath>

class MathFunction {};
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Vector3 Normalize(Vector3& pos);