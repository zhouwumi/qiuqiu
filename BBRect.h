#pragma once
class BBRect
{
public:
	BBRect();
	BBRect(int minX, float minY, int maxX, int maxY);
	BBRect(const BBRect& other);
	BBRect& operator= (const BBRect& other);
	void setRect(int minX, int minY, int maxX, int maxY);
	~BBRect();

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

