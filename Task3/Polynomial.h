#pragma once
#include"ListForPolynomial.h"

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




class Polynomial : public ListForPolynomial
{
private:
	
	void Create();
	void Destruct();


public:
	Polynomial();
	Polynomial(const char*);
	Polynomial(const Polynomial&);
	Polynomial(Polynomial&&);
	
	~Polynomial();

	Polynomial& operator=(const Polynomial&);
	Polynomial& operator=(Polynomial&&);

	Polynomial& operator+=(const Polynomial&);
	Polynomial& operator-=(const Polynomial&);
	Polynomial& operator*=(const Polynomial&);

	bool operator==(const Polynomial&);
	bool operator!=(const Polynomial&);

	friend std::ostream& operator<<(std::ostream&, const Polynomial&);
	friend std::istream& operator>>(std::istream&, Polynomial&);

	friend Polynomial operator*(const Polynomial&, const ListForPolynomialItem&);
};

Polynomial operator+(const Polynomial&, const Polynomial&);
Polynomial operator-(const Polynomial&, const Polynomial&);
Polynomial operator*(const Polynomial&, const Polynomial&);