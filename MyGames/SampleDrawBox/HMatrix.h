#pragma once

#include "HVector.h"

struct float4x4
{
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

class HMatrix : public float4x4
{
public:
	static  float g_identity[16];

public:
	HMatrix()
	{
		Identity();
	};
	void	Identity()
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

	HMatrix operator * (HMatrix const &Matrix)
	{
		HMatrix OutMatrix;
		for (int iColumn = 0; iColumn < 4; iColumn++)
		{
			for (int iRow = 0; iRow < 4; iRow++)
			{
				OutMatrix.m[iRow][iColumn] = m[iRow][0] * Matrix.m[0][iColumn] +
					m[iRow][1] * Matrix.m[1][iColumn] +
					m[iRow][2] * Matrix.m[2][iColumn] +
					m[iRow][3] * Matrix.m[3][iColumn];
			}
		}
		return OutMatrix;
	}
	HVector3 operator * (HVector3 const &vector)
	{
		return HVector3(
			vector.x * _11 + vector.y*_21 + vector.z*_31 + _41,
			vector.x * _12 + vector.y*_22 + vector.z*_32 + _42,
			vector.x * _13 + vector.y*_23 + vector.z*_33 + _43);
	}
	// access grants
	float& operator () (UINT iRow, UINT iCol)
	{
		return m[iRow][iCol];
	}
	float operator () (UINT iRow, UINT iCol) const
	{
		return m[iRow][iCol];
	}
	// casting operators
	operator float* ()
	{
		return (float *)&_11;
	}
	operator const float* () const
	{
		return (const float *)&_11;
	}
	void Translation(const HVector3 &vector)
	{
		_41 = vector.x;
		_42 = vector.y;
		_43 = vector.z;
	}
	void Translation(float x, float y, float z)
	{
		_41 = x;
		_42 = y;
		_43 = z;
	}
	void Scale(const HVector3 &vector)
	{
		_11 = vector.x;
		_22 = vector.y;
		_33 = vector.z;
	}
	void Scale(float x, float y, float z)
	{
		_11 = x;
		_22 = y;
		_33 = z;
	}
	void XRotate(float fRadian)
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_22 = fCosTheta;
		_23 = fSinTheta;
		_32 = -fSinTheta;
		_33 = fCosTheta;
	}
	void YRotate(float fRadian)
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = fCosTheta;
		_13 = -fSinTheta;
		_31 = fSinTheta;
		_33 = fCosTheta;
	}
	void ZRotate(float fRadian)
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = fCosTheta;
		_12 = fSinTheta;
		_21 = -fSinTheta;
		_22 = fCosTheta;
	}
	// 전치 행렬 계산
	HMatrix Transpose()
	{
		HMatrix matrix;
		matrix._11 = _11; matrix._21 = _21; matrix._13 = _31; matrix._14 = _41;
		matrix._21 = _12; matrix._22 = _22; matrix._23 = _32; matrix._24 = _42;
		matrix._31 = _13; matrix._32 = _23; matrix._33 = _33; matrix._34 = _43;
		matrix._41 = _14; matrix._42 = _24; matrix._43 = _34; matrix._44 = _44;
		return matrix;
	}
	//---------------------------------------------------------
	//           뷰 행렬 생성 
	//---------------------------------------------------------
	HMatrix ViewLookAt(HVector3& vPosition,
		HVector3& vTarget,
		HVector3& vUp)
	{
		HMatrix matrix;
		HVector3 vDirection = (vTarget - vPosition).Normal();
		HVector3 vRightVector = (vUp ^ vDirection).Normal();
		HVector3 vUpVector = (vDirection ^ vRightVector).Normal();

		_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
		_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
		_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;

		_41 = -(vPosition.x*_11 + vPosition.y*_21 + vPosition.z*_31);
		_42 = -(vPosition.x*_12 + vPosition.y*_22 + vPosition.z*_32);
		_43 = -(vPosition.x*_13 + vPosition.y*_23 + vPosition.z*_33);
		memcpy(matrix, this, 16 * sizeof(float));
		return matrix;
	}
	HMatrix CreateViewLook(HVector3& vPosition, HVector3& vTarget, HVector3& vUp)
	{
		HMatrix matrix;
		HVector3 vDirection = vTarget - vPosition;
		vDirection = vDirection.Normal();
		float fDot = vUp | vDirection;
		HVector3 vUpVector = vUp - (vDirection * fDot);
		vUpVector = vUpVector.Normal();
		HVector3 vRightVector = vUpVector ^ vDirection;

		_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
		_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
		_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;
		_41 = -(vPosition.x*_11 + vPosition.y*_21 + vPosition.z*_31);
		_42 = -(vPosition.x*_12 + vPosition.y*_22 + vPosition.z*_32);
		_43 = -(vPosition.x*_13 + vPosition.y*_23 + vPosition.z*_33);
		memcpy(matrix, this, 16 * sizeof(float));
		return matrix;
	}

	//        ^
	//        \
	//        |  \
	//        |    ---------------->
	// 내적의 결과는 직각이 된다.
	void ObjectLookAt(HVector3& vPosition,
		HVector3& vTarget,
		HVector3& vUp)
	{
		HVector3 vDirection = vTarget - vPosition;
		vDirection = vDirection.Normal();
		float fDot = vUp | vDirection;
		HVector3 vC = vDirection * fDot;
		HVector3 vUpVector = vUp - (vDirection * fDot);
		vUpVector = vUpVector.Normal();
		HVector3 vRightVector = vUpVector ^ vDirection;

		_11 = vRightVector.x;	_12 = vRightVector.y;	_13 = vRightVector.z;
		_21 = vUpVector.x;		_22 = vUpVector.y;		_23 = vUpVector.z;
		_31 = vDirection.x;		_32 = vDirection.y;		_33 = vDirection.z;
		_41 = vPosition.x;
		_42 = vPosition.y;
		_43 = vPosition.z;
	}
	// 역행렬 계산
	void	InverseGaussJordan(float* m, float *out);
	HMatrix	InverseGeneral(float *out = NULL);
	HMatrix	Inverse(float *out = NULL);

	// 뷰 행렬 계산
	HMatrix ComputeViewMatrix(HVector3& vPosition,
		HVector3& vTarget,
		HVector3& vUp)
	{
		HMatrix matrix;
		ObjectLookAt(vPosition, vTarget, vUp);
		_41 = _42 = _43 = 0.0f;
		//matrix = Inverse();
		matrix = Transpose();
		_41 = -(vPosition.x*matrix._11 + vPosition.y*matrix._21 + vPosition.z*matrix._31);
		_42 = -(vPosition.x*matrix._12 + vPosition.y*matrix._22 + vPosition.z*matrix._32);
		_43 = -(vPosition.x*matrix._13 + vPosition.y*matrix._23 + vPosition.z*matrix._33);
		memcpy(matrix, this, 16 * sizeof(float));
		return matrix;
	}
	//---------------------------------------------------------
	//           투영 행렬 생성 
	//---------------------------------------------------------

	// 원근 투영 행렬 계산
	// 원근 투영 행렬 계산
	HMatrix PerspectiveFovLH(float fNearPlane,
		float fFarPlane,
		float fovy,
		float Aspect) // width / heght
	{
		float    h, w, Q;

		h = 1 / tan(fovy*0.5f);  // 1/tans(x) = cot(x)
		w = h / Aspect;

		Q = fFarPlane / (fFarPlane - fNearPlane);

		HMatrix ret;
		ZeroMemory(this, sizeof(HMatrix));

		_11 = w;
		_22 = h;
		_33 = Q;
		_43 = -Q * fNearPlane;
		_34 = 1;

		memcpy(ret, this, 16 * sizeof(float));
		return ret;
	}
};