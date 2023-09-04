#pragma once
struct float4
{
	// x�����Ay�����Az�����Aw����
	float x, y, z, w;

	// �R���X�g���N�^
	float4();// -----------------------------------> �����Ȃ�
	float4(float x, float y, float z, float w);// -> ��������

	// �P�����Z�q�I�[�o�[���[�h
	float4 operator+() const;
	float4 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	float4& operator+=(const float4& f);
	float4& operator-=(const float4& f);
	float4& operator*=(float s);
	float4& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const float4 operator+(const float4& f1, const float4& f2);
const float4 operator-(const float4& f1, const float4& f2);
const float4 operator*(const float4& f, float s);
const float4 operator*(float s, const float4& f);
const float4 operator/(const float4& f, float s);