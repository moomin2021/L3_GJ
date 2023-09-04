#include "Matrix4.h"
#include "Vector3.h"
#include <cmath>

// --�P�ʍs������߂�-- //
Matrix4 Matrix4Identity() {
	static const Matrix4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

Matrix4 Matrix4Inverse(const Matrix4& m)
{
	Matrix4 result;
	float mat[4][8] = { 0 };

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			mat[i][j] = m.m[i][j];
		}
	}

	mat[0][4] = 1;
	mat[1][5] = 1;
	mat[2][6] = 1;
	mat[3][7] = 1;

	for (size_t n = 0; n < 4; n++) {
		//�ő�̐�Βl��T������(�Ƃ肠�����Ώې������ő�Ɖ��肵�Ă���)
		float max = abs(mat[n][n]);
		size_t maxIndex = n;

		for (size_t i = n + 1; i < 4; i++) {
			if (abs(mat[i][n]) > max) {
				max = abs(mat[i][n]);
				maxIndex = i;
			}
		}

		//�ő�̐�Βl��0��������t�s��͋��߂��Ȃ�
		if (abs(mat[maxIndex][n]) <= 0.000001f) {
			return result; //�Ƃ肠�����P�ʍs��Ԃ����Ⴄ
		}

		//����ւ�
		if (n != maxIndex) {
			for (size_t i = 0; i < 8; i++) {
				float f = mat[maxIndex][i];
				mat[maxIndex][i] = mat[n][i];
				mat[n][i] = f;
			}
		}

		//�|������1�ɂȂ�l���Z�o
		float mul = 1 / mat[n][n];

		//�|����
		for (size_t i = 0; i < 8; i++) {
			mat[n][i] *= mul;
		}

		//���S��0�ɂ���
		for (size_t i = 0; i < 4; i++) {
			if (n == i) {
				continue;
			}

			float mul = -mat[i][n];

			for (size_t j = 0; j < 8; j++) {
				mat[i][j] += mat[n][j] * mul;
			}
		}
	}

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = mat[i][j + 4];
		}
	}

	return result;
}

// --�g��k���s������߂�-- //
Matrix4 Matrix4Scale(const Vector3& s) {
	Matrix4 result{
		s.x, 0.0f, 0.0f, 0.0f,
		0.0f, s.y, 0.0f, 0.0f,
		0.0f, 0.0f, s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// --X���܂��̉�]�s������߂�-- //
Matrix4 Matrix4RotateX(float angle) {
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos, sin, 0.0f,
		0.0f, -sin, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// --Y���܂��̉�]�s������߂�-- //
Matrix4 Matrix4RotateY(float angle) {
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result{
		cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin, 0.0f, cos, 0.0f,
		0.0f, 0.0, 0.0f, 1.0f
	};

	return result;
}

// --Z���܂��̉�]�s������߂�-- //
Matrix4 Matrix4RotateZ(float angle) {
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result{
		cos, sin, 0.0f, 0.0f,
		-sin, cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0, 0.0f, 1.0f
	};

	return result;
}

// --���s�ړ��s������߂�-- //
Matrix4 Matrix4Translate(const Vector3& t) {
	Matrix4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x, t.y, t.z, 1.0f
	};

	return result;
}

// --���W�ϊ��i�x�N�g���ƍs��̊|���Z�j���s��transform�֐����쐬����B�i�����ϊ��ɂ��Ή����Ă���j-- //
Vector3 Matrix4Transform(const Vector3& v, const Matrix4& m) {
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

// --������Z�q *= �̃I�[�o�[���[�h�֐��i�s��ƍs��̐ρj-- //
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2) {
	Matrix4 result{ 0 };

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;
	return m1;
}

// --2�����Z�q*�̃I�[�o�[���[�h�֐��i�s��ƍs��̐ρj-- //
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) {
	Matrix4 result = m1;
	return result *= m2;
}

// --2�����Z�q*�̃I�[�o�[���[�h�֐��i�x�N�g���ƍs��̐ρj-- //
const Vector3 operator*(const Vector3& v, const Matrix4& m) {
	return Matrix4Transform(v, m);
}