#pragma once
#include"mathUtil.h"
#include"VECTOR.h"

// (Unit) QUATERNION
class QUATERNION
{
public:
	float x;
	float y;
	float z;
	float w;

	QUATERNION()
	{
		*this = QUATERNION::Identity;
	}

	//クォータニオン成分を直接設定する
	explicit QUATERNION(float inX, float inY, float inZ, float inW)
	{
		Set(inX, inY, inZ, inW);
	}

	//軸と角度からクォータニオンを構築する
	//軸はすでに正規化されているものとする
	explicit QUATERNION(const VECTOR& axis, float angle)
	{
		float scalar = Sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Cos(angle / 2.0f);
	}

	void Set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	void Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float LengthSq() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	float Length() const
	{
		return Sqrt(LengthSq());
	}

	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	//正規化
	static QUATERNION Normalize(const QUATERNION& q)
	{
		QUATERNION retVal = q;
		retVal.Normalize();
		return retVal;
	}

	//線形保管
	static QUATERNION Lerp(const QUATERNION& a, const QUATERNION& b, float f)
	{
		QUATERNION retVal;
		retVal.x = lerp(a.x, b.x, f);
		retVal.y = lerp(a.y, b.y, f);
		retVal.z = lerp(a.z, b.z, f);
		retVal.w = lerp(a.w, b.w, f);
		retVal.Normalize();
		return retVal;
	}

	static float Dot(const QUATERNION& a, const QUATERNION& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	//球面線形補完
	static QUATERNION Slerp(const QUATERNION& a, const QUATERNION& b, float f)
	{
		float rawCosm = QUATERNION::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;

		if (cosom < 0.9999f)
		{
			const float omega = Acos(cosom);
			const float invSin = 1.f / Sin(omega);
			scale0 = Sin((1.f - f) * omega) * invSin;
			scale1 = Sin(f * omega) * invSin;
		}
		else
		{
			//もしクォータニオンが同一線上にあるなら、線形補完でいく
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		QUATERNION retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.Normalize();
		return retVal;
	}

	// 結合
	// 先にｐ、後でｑの順に回転
	static QUATERNION Concatenate(const QUATERNION& q, const QUATERNION& p)
	{
		QUATERNION retVal;

		// ベクター成分は：ps * qv + qs * pv + pv x qv
		VECTOR qv(q.x, q.y, q.z);
		VECTOR pv(p.x, p.y, p.z);
		VECTOR newVec = p.w * qv + q.w * pv + cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		// スカラー成分は：ps * qs - pv . qv
		retVal.w = p.w * q.w - dot(pv, qv);

		return retVal;
	}

	static const QUATERNION Identity;
};
