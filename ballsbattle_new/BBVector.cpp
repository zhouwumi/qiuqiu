#include "BBVector.h"
#include <math.h>
#include "BBMath.h"

BBVector::BBVector()
{
	SetPoint(0, 0);
}

BBVector::BBVector(float _x, float _y)
{
	SetPoint(_x, _y);
}

BBVector::BBVector(const BBVector& other)
{
	SetPoint(other.x, other.y);
}

BBVector& BBVector::operator= (const BBVector& other)
{
	SetPoint(other.x, other.y);
	return *this;
}

float BBVector::Dot(const BBVector& v) const
{
	return x * v.x + y * v.y;
}

float BBVector::Dot(const BBVector& v1, const BBVector& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float BBVector::GetAngle()
{
	if (*this == BBVector::ZERO)
	{
		return 0;
	}
	float result = 180 * atan2f(this->y, this->x) / BB_M_PI;
	if (result < 0)
	{
		result += 360;
	}
	return result;
}

BBVector BBVector::GetFixedVetor2(BBVector v, float length)
{
	if (v == BBVector::ZERO)
	{
		return BBVector::ZERO;
	}
	/*float radian;
	float absRadian = atan2f(fabs(v.y), fabs(v.x));
	if (v.x >= 0 && v.y >= 0) {
		radian = absRadian;
	}
	else if (v.x <= 0 && v.y >= 0) {
		radian = BB_M_PI - absRadian;
	}
	else if (v.x <= 0 && v.y <= 0) {
		radian = BB_M_PI + absRadian;
	}
	else {
		radian = 2 * BB_M_PI - absRadian;
	}
	return BBVector(cosf(radian) * length, sinf(radian) * length);*/
	float vLen = sqrtf(v.x * v.x + v.y * v.y);
	float xRate = v.x / vLen;
	float yRate = v.y / vLen;
	int x = xRate * length;
	int y = yRate * length;
	return BBVector(x, y);
}

BBVector BBVector::GetDeltaVector(BBVector startVec, BBVector endVec, int frame)
{
	if (frame < 1)
	{
		frame = 1;
	}
	return BBVector((endVec.x - startVec.x) / frame, (endVec.y - startVec.y) / frame);
}

void BBVector::SetPoint(float _x, float _y)
{
	x = _x;
	y = _y;
}

void BBVector::Normalize()
{
	float n = x * x + y * y;
	if (n == 1.0f) {
		return;
	}
	n = sqrt(n);
	if (n < BB_MATH_TOLERANCE)
		return;
	n = 1.0f / n;
	x *= n;
	y *= n;
}

BBVector BBVector::GetNormalized() const
{
	BBVector v(*this);
	v.Normalize();
	return v;
}

void BBVector::Negate()
{
	x = -x;
	y = -y;
}

void BBVector::Scale(float scalar)
{
	x *= scalar;
	y *= scalar;
}

void BBVector::Subtract(const BBVector& v)
{
	x -= v.x;
	y -= v.y;
}

void BBVector::Add(const BBVector& v)
{
	x += v.x;
	y += v.y;
}

BBVector BBVector::operator+(const BBVector& v) const
{
	BBVector result(*this);
	result.Add(v);
	return result;
}

BBVector& BBVector::operator+=(const BBVector& v)
{
	Add(v);
	return *this;
}

BBVector BBVector::operator-(const BBVector& v) const
{
	BBVector result(*this);
	result.Subtract(v);
	return result;
}

BBVector& BBVector::operator-=(const BBVector& v)
{
	Subtract(v);
	return *this;
}

BBVector BBVector::operator-() const
{
	BBVector result(*this);
	result.Negate();
	return result;
}

BBVector BBVector::operator*(float s) const
{
	BBVector result(*this);
	result.Scale(s);
	return result;
}

BBVector& BBVector::operator*=(float s)
{
	Scale(s);
	return *this;
}

BBVector BBVector::operator/(float s) const
{
	return BBVector(x / s, y / s);
}

bool BBVector::operator==(const BBVector& v) const
{
	return x == v.x && y == v.y;
}

bool BBVector::operator!=(const BBVector& v) const
{
	return x != v.x || y != v.y;
}

BBVector::~BBVector()
{

}

const BBVector BBVector::ZERO = BBVector(0.0f, 0.0f);