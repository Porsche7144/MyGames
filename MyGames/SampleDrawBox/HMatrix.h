#pragma once
#include "HVector.h"

struct float4x4
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float mat[4][4];
	};
};

class HMatrix : float4x4
{
public:
	static float g_identity[16];

public:
	HMatrix()
	{
		Identity();
	}

	void Identity()
	{
		_12 = _13 = _14 = 0.0f;
		_21 = _23 = _24 = 0.0f;
		_31 = _32 = _34 = 0.0f;
		_41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}

	HMatrix(const HMatrix& matrix)
	{
		_11 = matrix._11; _12 = matrix._12; _13 = matrix._13; _14 = matrix._14;
		_21 = matrix._21; _22 = matrix._22; _23 = matrix._23; _24 = matrix._24;
		_31 = matrix._31; _32 = matrix._32; _33 = matrix._33; _34 = matrix._34;
		_41 = matrix._41; _42 = matrix._42; _43 = matrix._43; _44 = matrix._44;
	}

	HMatrix operator * (HMatrix const & matrix)
	{
		HMatrix OutMatrix;
		for (int iCol = 0; iCol < 4; iCol++)
		{
			for (int iRow = 0; iRow < 4; iRow++)
			{
				OutMatrix.mat[iRow][iCol] = mat[iRow][0] * matrix.mat[0][iCol] +
					mat[iRow][1] * matrix.mat[1][iCol] +
					mat[iRow][2] * matrix.mat[2][iCol] +
					mat[iRow][3] * matrix.mat[3][iCol];

				return OutMatrix;
			}
		}
	}

	HVector3 operator * (HVector3 const &vector)
	{
		return HVector3(vector.x * _11 + vector.y * _21 + vector.z * _31 + _41,
						vector.x * _12 + vector.y * _22 + vector.z * _32 + _42,
						vector.x * _13 + vector.y * _23 + vector.z * _33 + _43);
	}

	// 전치행렬
	HMatrix Transpose()
	{
		HMatrix matrix;
		matrix._11 = _11; matrix._12 = _21; matrix._13 = _31; matrix._14 = _41;
		matrix._21 = _12; matrix._22 = 22; matrix._23 = _32; matrix._24 = _42;
		matrix._31 = _13; matrix._32 = _23; matrix._33 = _33; matrix._34 = _43;
		matrix._41 = _14; matrix._42 = _24; matrix._43 = _34; matrix._44 = _44;

		return matrix;
	}

	HMatrix VIewLookAt(HVector3& pos, HVector3 target, HVector3 up)
	{
		HMatrix matrix;
		HVector3 vDirection = (target - pos).
	}
};

