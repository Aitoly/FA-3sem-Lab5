#pragma once
#include"List.h"

class PolynomialExeption
{
private:
	char* error;
	PolynomialExeption();


public:
	PolynomialExeption(const char*);
	PolynomialExeption(const PolynomialExeption&);
	~PolynomialExeption();

	const char* what();
};

class Polynomial : public List
{
private:
	
	void Create();
	void Destruct();
	void SizeTo2();
	Polynomial karatsuba_mul(const Polynomial&) const;
	Polynomial(const Polynomial&, int, int);


public:
	Polynomial();
	explicit Polynomial(int, double, ...);
	explicit Polynomial(char*);
	Polynomial(const Polynomial&);
	
	Polynomial(Polynomial&&);
	
	~Polynomial();

	void Normal();
	void SizeToUp(int);
	void SizeToEnd(int);
	double Solve(double);
	Polynomial Derivative();

	Polynomial& operator=(const Polynomial&);
	Polynomial& operator=(Polynomial&&);

	Polynomial& operator+=(const Polynomial&);
	Polynomial& operator-=(const Polynomial&);
	Polynomial& operator*=(const Polynomial&);//
	Polynomial& operator/=(Polynomial&);//
	Polynomial& operator%=(const Polynomial&);//

	Polynomial& operator*=(double);
	Polynomial& operator/=(double);

	friend std::ostream& operator<<(std::ostream&, const Polynomial&);
	friend std::istream& operator>>(std::istream&, Polynomial&);

	friend Polynomial operator*(const Polynomial&, const Polynomial&);
};

Polynomial operator+(const Polynomial&, const Polynomial&);
Polynomial operator-(const Polynomial&, const Polynomial&);
Polynomial operator/(const Polynomial&, Polynomial&);
Polynomial operator%(const Polynomial&, const Polynomial&);

Polynomial operator*(const Polynomial&, double);
Polynomial operator/(const Polynomial&, double);