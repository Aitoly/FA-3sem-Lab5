#pragma once




class LargeIntegerEx�eption
{
private:
	char* error;
	LargeIntegerEx�eption();


public:
	LargeIntegerEx�eption(const char*);
	LargeIntegerEx�eption(const LargeIntegerEx�eption&);
	~LargeIntegerEx�eption();

	const char* what();
};







class LargeInteger
{
private:
	int* data;
	int size;
	int base;
	bool plus;

	LargeInteger(const LargeInteger&, bool);
	LargeInteger(const LargeInteger&, int, int);

	void SizeToUp(int);
	void SizeToEnd(int);
	void SizeTo2();
	void Normal();
	void Swap(LargeInteger&);
	LargeInteger Mul(const LargeInteger&) const;
	LargeInteger KaratsubaMul(const LargeInteger&) const;
	
	LargeInteger& operator*=(int);
	

	int Mod(int);

public:
	LargeInteger MulInt(int);

	LargeInteger();
	LargeInteger(int);
	LargeInteger(const char*, int base = 10);
	LargeInteger(const LargeInteger&);
	LargeInteger(LargeInteger&&);
	~LargeInteger();

	LargeInteger& operator=(const LargeInteger&);
	LargeInteger& operator=(LargeInteger&&);

	LargeInteger& operator+=(const LargeInteger&);
	LargeInteger& operator-=(const LargeInteger&);
	LargeInteger& operator*=(const LargeInteger&);
	LargeInteger& operator/=(const LargeInteger&);
	LargeInteger& operator%=(const LargeInteger&);
	
	

	bool operator<(LargeInteger&);
	bool operator==(LargeInteger&);
	bool operator>(LargeInteger&);
	bool operator>=(LargeInteger&);

	friend std::ostream& operator<<(std::ostream&, const LargeInteger&);
	friend std::istream& operator>>(std::istream&, LargeInteger&);

	LargeInteger Pow(int);

	LargeInteger TranslateTo10();//TODO  ����� �������� ����� �� �������� ������� ��������� k � 10 - �� ������� ���������
	LargeInteger TranslateTo(int);//TODO   ����� �������� ��������� ������ ����� � ������� ��������� � ���������� k
};

LargeInteger operator+(const LargeInteger&, const LargeInteger&);
LargeInteger operator-(const LargeInteger&, const LargeInteger&);
LargeInteger operator*(const LargeInteger&, const LargeInteger&);
LargeInteger operator/(const LargeInteger&, const LargeInteger&);
LargeInteger operator%(const LargeInteger&, const LargeInteger&);

LargeInteger Fibonacci(int);
LargeInteger Factorial(int);
LargeInteger BinomialCoeff(int, int);