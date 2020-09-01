#pragma once
#include"ListForMonomial.h"


class MonomialEx�eption
{
private:
	char* error;
	MonomialEx�eption();


public:
	MonomialEx�eption(const char*);
	MonomialEx�eption(const MonomialEx�eption&);
	~MonomialEx�eption();

	const char* what();
};







class Monomial : public ListForMonomial
{
private:
	double koeff;

public:
	Monomial();
	Monomial(char*);
	Monomial(const Monomial&);
	Monomial(Monomial&&);

	Monomial& operator=(const Monomial&);
	Monomial& operator=(Monomial&&);

	Monomial& operator*=(const Monomial&);
	Monomial& operator/=(const Monomial&);

	bool operator<(const Monomial&);
	bool operator==(const Monomial&);
	bool operator!=(const Monomial&);
	bool operator>(const Monomial&);

	double GetKoeff() const;
	void SetKoeff(double);

	friend std::ostream& operator<<(std::ostream&, const Monomial&);
	friend std::istream& operator>>(std::istream&, Monomial&);

	~Monomial();
};

Monomial operator*(const Monomial&, const Monomial&);
Monomial operator/(const Monomial&, const Monomial&);