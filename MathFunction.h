#pragma once
#include "Matrix.h"
#include "Vector3.h"

class MathFunction {};
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
