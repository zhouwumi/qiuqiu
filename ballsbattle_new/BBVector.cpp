#include "BBVector.h"
#include <math.h>
#include "BBMath.h"

BBVector::BBVector()
{
	SetPoint(0, 0);
}

BBVector::BBVector(double _x, double _y)
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

double BBVector::Dot(const BBVector& v) const
{
	return x * v.x + y * v.y;
}

double BBVector::Dot(const BBVector& v1, const BBVector& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

double BBVector::GetAngle()
{
	if (*this == BBVector::ZERO)
	{
		return 0;
	}
	double result = 180 * atan2f(this->y, this->x) / BB_M_PI;
	if (result < 0)
	{
		result += 360;
	}
	return result;
}

BBVector BBVector::GetFixedVetor2(BBVector v, double length)
{
	if (v == BBVector::ZERO)
	{
		return BBVector::ZERO;
	}
	/*double radian;
	double absRadian = atan2f(fabs(v.y), fabs(v.x));
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
	double vLen = sqrtf(v.x * v.x + v.y * v.y);
	double xRate = v.x / vLen;
	double yRate = v.y / vLen;
	return BBVector(xRate * length, yRate * length);
}

BBVector BBVector::GetDeltaVector(BBVector startVec, BBVector endVec, int frame)
{
	if (frame < 1)
	{
		frame = 1;
	}
	return BBVector((endVec.x - startVec.x) / frame, (endVec.y - startVec.y) / frame);
}

void BBVector::SetPoint(double _x, double _y)
{
	x = _x;
	y = _y;
}

void BBVector::Normalize()
{
	double n = x * x + y * y;
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

void BBVector::Scale(double scalar)
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

BBVector BBVector::operator*(double s) const
{
	BBVector result(*this);
	result.Scale(s);
	return result;
}

BBVector& BBVector::operator*=(double s)
{
	Scale(s);
	return *this;
}

BBVector BBVector::operator/(double s) const
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