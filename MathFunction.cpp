#include "MathFunction.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}

Vector3 Normalize(Vector3& pos) { 
	Vector3 newPos = pos;
	float length = sqrt((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z));
	if (length != 0.0f) {
		newPos.x = pos.x / length;
		newPos.y = pos.y / length;
		newPos.z = pos.z / length;
	}
	return newPos;
}