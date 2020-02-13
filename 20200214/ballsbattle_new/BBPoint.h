#ifndef BB_POINT_CPP
#define BB_POINT_CPP
class BBPoint
{
public:
	BBPoint();
	BBPoint(int _x, int _y);
	BBPoint(const BBPoint& other);
	BBPoint& operator= (const BBPoint& other);

	void SetPoint(int _x, int _y);
	static const BBPoint ZERO;
public:
	int x;
	int y;
};

#endif
