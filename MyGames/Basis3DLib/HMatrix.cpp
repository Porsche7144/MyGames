#include "HMatrix.h"

float HMatrix::g_identity[16] =
{
   1.0f, 0.0f, 0.0f, 0.0f,
   0.0f, 1.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 1.0f, 0.0f,
   0.0f, 0.0f, 0.0f, 1.0f
};


//1.0f	미지수의 개수와 행렬의 계수를 입력받는다. 
//2.	행렬의 대각값이 가장 커지도록, 행을 교환한다. 
//3.	식 (3)의 확대행렬을 구성한다. 
//4.	Gauss-Jordan법을 이용하여 식(4)의 역행렬을 계산한다. 
//5.	계산된 역행렬로 해를 구한다.
//http://matrix.skku.ac.kr/sglee/linear/ocu/dfn.html#정의 1
//http://matrix.skku.ac.kr/MT-04/MTFAQ/probFAQ.htm
void HMatrix::InverseGaussJordan(float* m, float *out)
{
	double a[4][8];
	int numrow = 4;

	for (int i = 0; i < numrow; i++)
	{
		for (int j = 0; j < numrow; j++)
		{
			a[i][j] = m[i * 4 + j];
			if (i == j)
				a[i][j + numrow] = 1.0f;
			else
				a[i][j + numrow] = 0.0f;
		}
	}

	/*..pivot..*/
	for (int i = 0; i < numrow - 1; i++)
	{
		int irow = i;
		for (int k = i + 1; k < numrow; k++)
		{
			// 0, 0 
			// 1, 0 
			// 2, 0
			// 3, 0:
			if (fabs(a[irow][i]) < fabs(a[k][i]))
			{
				irow = k;
			}
		}
		// 두 행을 교체
		for (int j = 0; j < 2 * numrow; j++)
		{
			float dtemp = a[i][j];
			a[i][j] = a[irow][j];
			a[irow][j] = dtemp;
		}
	}

	/*..Gauss-Jordan..*/
	for (int i = 0; i < numrow; i++)
	{
		float pivot = a[i][i];
		for (int j = 0; j < 2 * numrow; j++)
		{
			a[i][j] /= pivot;
		}
		for (int k = 0; k < numrow; k++)
		{
			if (k == i) continue;
			float dtemp = a[k][i];
			for (int j = i; j < 2 * numrow; j++)
			{
				a[k][j] -= dtemp * a[i][j];
			}
		}
	}

	/*..Inverse Matrix..*/
	for (int i = 0; i < numrow; i++)
	{
		for (int j = 0; j < numrow; j++)
		{
			out[j + 4 * i] = a[i][j + numrow];
		}
	}
}

HMatrix  HMatrix::InverseGeneral(float *out)
{
	HMatrix matInverse;
	float det;
	float d12, d13, d23, d24, d34, d41;
	float tmp[16]; /* Allow out == in. */

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	/* pre-compute 2x2 dets for last two rows when computing */
	/* cofactors of first two rows. */
	d12 = (_31*_42 - _41 * _32);
	d13 = (_31*_43 - _41 * _33);
	d23 = (_32*_43 - _42 * _33);
	d24 = (_32*_44 - _42 * _34);
	d34 = (_33*_44 - _43 * _34);
	d41 = (_34*_41 - _44 * _31);

	tmp[0] = (_22 * d34 - _23 * d24 + _24 * d23);
	tmp[1] = -(_21 * d34 + _23 * d41 + _24 * d13);
	tmp[2] = (_21 * d24 + _22 * d41 + _24 * d12);
	tmp[3] = -(_21 * d23 - _22 * d13 + _23 * d12);

	/* Compute determinant as early as possible using these cofactors. */
	det = _11 * tmp[0] + _12 * tmp[1] + _13 * tmp[2] + _14 * tmp[3];

	/* Run singularity test. */
	if (det == 0.0f)
	{
		if (out != NULL)
		{
			memcpy(out, g_identity, 16 * sizeof(float));
		}
	}
	else
	{
		float invDet = 1.0f / det;
		/* Compute rest of inverse. */
		tmp[0] *= invDet;
		tmp[1] *= invDet;
		tmp[2] *= invDet;
		tmp[3] *= invDet;

		tmp[4] = -(_12 * d34 - _13 * d24 + _14 * d23) * invDet;
		tmp[5] = (_11 * d34 + _13 * d41 + _14 * d13) * invDet;
		tmp[6] = -(_11 * d24 + _12 * d41 + _14 * d12) * invDet;
		tmp[7] = (_11 * d23 - _12 * d13 + _13 * d12) * invDet;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = _11 * _22 - _21 * _12;
		d13 = _11 * _23 - _21 * _13;
		d23 = _12 * _23 - _22 * _13;
		d24 = _12 * _24 - _22 * _14;
		d34 = _13 * _24 - _23 * _14;
		d41 = _14 * _21 - _24 * _11;

		tmp[8] = (_42 * d34 - _43 * d24 + _44 * d23) * invDet;
		tmp[9] = -(_41 * d34 + _43 * d41 + _44 * d13) * invDet;
		tmp[10] = (_41 * d24 + _42 * d41 + _44 * d12) * invDet;
		tmp[11] = -(_41 * d23 - _42 * d13 + _43 * d12) * invDet;
		tmp[12] = -(_32 * d34 - _33 * d24 + _34 * d23) * invDet;
		tmp[13] = (_31 * d34 + _33 * d41 + _34 * d13) * invDet;
		tmp[14] = -(_31 * d24 + _32 * d41 + _34 * d12) * invDet;
		tmp[15] = (_31 * d23 - _32 * d13 + _33 * d12) * invDet;

		memcpy(matInverse, tmp, 16 * sizeof(float));
		matInverse = matInverse.Transpose();
	}
	if (out != NULL)
	{
		memcpy(out, tmp, 16 * sizeof(float));
	}
	return matInverse;
}

HMatrix  HMatrix::Inverse(float *out)
{
	HMatrix matInverse;
	float det;
	float tmp[16]; /* Allow out == in. */

	if (_41 != 0.0f || _42 != 0.0f || _43 != 0.0f || _44 != 1.0f)
	{
		return InverseGeneral(out);
	}

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	tmp[0] = _22 * _33 - _23 * _32;
	tmp[1] = _23 * _31 - _21 * _33;
	tmp[2] = _21 * _32 - _22 * _31;

	/* Compute determinant as early as possible using these cofactors. */
	det = _11 * tmp[0] + _12 * tmp[1] + _13 * tmp[2];

	/* Run singularity test. */
	if (det == 0.0f)
	{
		if (out != NULL)
		{
			memcpy(out, g_identity, 16 * sizeof(float));
		}
	}
	else
	{
		float d12, d13, d23, d24, d34, d41;
		register float im11, im12, im13, im14;

		det = 1.0f / det;

		/* Compute rest of inverse. */
		tmp[0] *= det;
		tmp[1] *= det;
		tmp[2] *= det;
		tmp[3] = 0.0f;

		im11 = _11 * det;
		im12 = _12 * det;
		im13 = _13 * det;
		im14 = _14 * det;
		tmp[4] = im13 * _32 - im12 * _33;
		tmp[5] = im11 * _33 - im13 * _31;
		tmp[6] = im12 * _31 - im11 * _32;
		tmp[7] = 0.0f;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = im11 * _22 - _21 * im12;
		d13 = im11 * _23 - _21 * im13;
		d23 = im12 * _23 - _22 * im13;
		d24 = im12 * _24 - _22 * im14;
		d34 = im13 * _24 - _23 * im14;
		d41 = im14 * _21 - _24 * im11;

		tmp[8] = d23;
		tmp[9] = -d13;
		tmp[10] = d12;
		tmp[11] = 0.0f;

		tmp[12] = -(_32 * d34 - _33 * d24 + _34 * d23);
		tmp[13] = (_31 * d34 + _33 * d41 + _34 * d13);
		tmp[14] = -(_31 * d24 + _32 * d41 + _34 * d12);
		tmp[15] = 1.0f;
		memcpy(matInverse, tmp, 16 * sizeof(float));
		matInverse = matInverse.Transpose();
	}
	if (out != NULL)
	{
		memcpy(out, tmp, 16 * sizeof(float));
	}
	return matInverse;
}
