#ifndef BB_VECTOR_CPP
#define BB_VECTOR_CPP
class BBVector
{
public:
	BBVector();
	BBVector(double _x, double _y);
	BBVector(const BBVector& other);
	BBVector& operator= (const BBVector& other);

	~BBVector();

	double Dot(const BBVector& v) const;
	static double Dot(const BBVector& v1, const BBVector& v2);

	double GetAngle();

	static BBVector GetFixedVetor2(BBVector v, double length);
	static BBVector GetDeltaVector(BBVector startVec, BBVector endVec, int frame);

	void Negate();
	void Normalize();
	BBVector GetNormalized() const;

	void Scale(double scalar);
	void Subtract(const BBVector& v);
	void Add(const BBVector& v);

	inline BBVector operator+(const BBVector& v) const;

	inline BBVector& operator+=(const BBVector& v);
	
	inline BBVector operator-(const BBVector& v) const;

	inline BBVector& operator-=(const BBVector& v);

	inline BBVector operator-() const;

	inline BBVector operator*(double s) const;

	 BBVector& operator*=(double s);

	inline BBVector operator/(double s) const;

	 bool operator==(const BBVector& v) const;
	 bool operator!=(const BBVector& v) const;

	void SetPoint(double _x, double _y);

	static const BBVector ZERO;
public:
	double x;
	double y;
};

#endif