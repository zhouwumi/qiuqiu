#ifndef BB_RECT_CPP
#define BB_RECT_CPP
#include "BBPoint.h"

class BBRect
{
public:
	BBRect();
	BBRect(int minX, double minY, int maxX, int maxY);
	BBRect(const BBRect& other);
	BBRect& operator= (const BBRect& other);
	void setRect(int minX, int minY, int maxX, int maxY);

	BBRect expand(int expandW, int expandH);
	bool intersectsRect(const BBRect& rect) const;
	bool intersctsRect(int minX, int maxX, int minY, int maxY) const;
	bool intersectsCircle(const BBPoint& center, int radius) const;

	~BBRect() = default;

	int GetMinX();
	int GetMaxX();
	int GetMinY();
	int GetMaxY();

	int GetCenterX();
	int GetCenterY();

	static const BBRect ZERO;
public:
	int minX;
	int maxX;
	int minY;
	int maxY;
	int centerX;
	int centerY;
};

#endif

