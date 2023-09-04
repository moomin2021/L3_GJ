#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>    // sqrt

// �R���X�g���N�^
Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// �����o�֐�

// �m����(����)�����߂�
float Vector3::length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

// ���ς����߂�
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

// �O�ς����߂�
Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

// �P�����Z�q�I�[�o�[���[�h
Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

// ������Z�q�I�[�o�[���[�h
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3 Vector3Normalize(const Vector3& vec)
{
	float len = vec.length();
	if (len != 0)
	{
		return vec / len;
	}
	return vec;
}

Vector3 Vector3Cross(const Vector3& v0, const Vector3& v1)
{
	return Vector3(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
}

float Vector3Dot(const Vector3& v0, const Vector3& v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

Vector3 Vector3Transform(const Vector3& v, const Matrix4& m) {
	Vector3 result = {
		m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z
	};

	return result;
}

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}


const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}