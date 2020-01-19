#ifndef BB_VECTOR_CPP
#define BB_VECTOR_CPP
class BBVector
{
public:
	BBVector();
	BBVector(float _x, float _y);
	BBVector(const BBVector& other);
	BBVector& operator= (const BBVector& other);

	~BBVector();

	float Dot(const BBVector& v) const;
	static float Dot(const BBVector& v1, const BBVector& v2);

	float GetAngle();

	static BBVector GetFixedVetor2(BBVector v, float length);
	static BBVector GetDeltaVector(BBVector startVec, BBVector endVec, int frame);

	void Negate();
	void Normalize();
	BBVector GetNormalized() const;

	void Scale(float scalar);
	void Subtract(const BBVector& v);
	void Add(const BBVector& v);

	inline BBVector operator+(const BBVector& v) const;

	inline BBVector& operator+=(const BBVector& v);
	
	inline BBVector operator-(const BBVector& v) const;

	inline BBVector& operator-=(const BBVector& v);

	inline BBVector operator-() const;

	inline BBVector operator*(float s) const;

	 BBVector& operator*=(float s);

	inline BBVector operator/(float s) const;

	 bool operator==(const BBVector& v) const;
	 bool operator!=(const BBVector& v) const;

	void SetPoint(float _x, float _y);

	static const BBVector ZERO;
public:
	float x;
	float y;
};

#endif