#pragma warning(disable:4996)
#include<iostream>
#include<stdarg.h>
#include<cstring>
#include<algorithm>

#include "Polynomial.h"

#define DEBUGj

////////////////////////////////////////////////////////
//		Исключения
////////////////////////////////////////////////////////

PolynomialExeption::PolynomialExeption()
:error(NULL)
{}

PolynomialExeption::PolynomialExeption(const char* error_)
{
	if (error_)
	{
		error = new char[strlen(error_) + 1];
		strcpy(error, error_);
	}
	else
	{
		error = NULL;
	}
}

PolynomialExeption::PolynomialExeption(const PolynomialExeption& Ex)
{
	if (Ex.error)
	{
		error = new char[strlen(Ex.error) + 1];
		strcpy(error, Ex.error);
	}
	else
	{
		error = NULL;
	}
}

PolynomialExeption::~PolynomialExeption()
{
	if (error)
		delete[] error;
}

const char* PolynomialExeption::what()
{
	return error;
}

////////////////////////////////////////////////////////
//		Конструкторы
////////////////////////////////////////////////////////
Polynomial::Polynomial()
{
}

Polynomial::Polynomial(int n, double first, ...)
{
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
	{
		Insert(va_arg(args, double));
	}
	va_end(args);
}

Polynomial::Polynomial(char* str)
{
	try
	{
		int tmp = 0;
		int len = strlen(str);
		char sign = '+';
		bool firstFinded = false, isPower = false, started = false;
		int power = 0, power_ = -1;


		head = new ListItem(0);
		head->SetNext(head);
		head->SetPrev(head);
		size = 0;

		if (*str == 'x')
		{
			tmp = 1;
		}

		for (int i = 0; i < len+1; ++i)
		{
			if (isspace(*str))
			{
				continue;
			}
			else if (*str == '\0' || *str == '-' || *str == '+' || *str == '^' || *str == '\n')
			{
			}
			else if (isdigit(*str))
			{
				if (isPower)
				{
					power *= 10;
					power += *str - '0';
				}
				else
				{
					tmp *= 10;
					tmp += *str - '0';
				}
			}
			else if (*str == 'x')
			{
				if (*(str + 1) == '^')
				{
					isPower = true;
				}
				else
				{
					power = 1;
				}
			}
			else
			{
				throw PolynomialExeption("Unexpected symbol");
			}



			if ((*str == '\0' || *str == '-' || *str == '+') && started)
			{
				if (!firstFinded)
				{
					power_ = power + 1;
					firstFinded = true;
				}

				if (power >= power_)
					throw PolynomialExeption("Wrong degree order");

				while (power_ - power != 1)
				{
					Insert(0);
					power_--;
				}

				if (sign == '-')
					tmp *= -1;

				Insert(tmp);

				power_ = power;
				tmp = 0;
				isPower = false;
				power = 0;
			}

			if (*str == '-' || *str == '+')
			{
				sign = *str;
				if (*(str + 1) == 'x')
				{
					tmp = 1;
				}
			}

			started = true;
			++str;
		}

		while (power_ != 0)
		{
			Insert(0);
			power_--;
		}

	}
	catch (PolynomialExeption &Ex)
	{
		std::cout << Ex.what() << std::endl;
		head = new ListItem(0);
		head->SetNext(head);
		head->SetPrev(head);
		size = 0;
	}
}

Polynomial::Polynomial(const Polynomial &polynomial)
{
	ListItem* item = polynomial.First();

	for (int i = 0; i < polynomial.size; ++i)
	{
		Insert(item->GetVal());
		item = item->GetNext();
	}
}

Polynomial::Polynomial(const Polynomial &polynomial, int begin, int end)
{
	if (begin < 0)
		begin = 0;

	if (begin > polynomial.size - 1)
		begin = polynomial.size - 1;

	if (end < 0)
		end = 0;

	if (end > polynomial.size)
		end = polynomial.size;

	ListItem* item = &polynomial[begin];

	for (int i = begin; i < end; ++i)
	{
		Insert(item->GetVal());
		item = item->GetNext();
	}
}

Polynomial::Polynomial(Polynomial &&polynomial)
{
	delete head;

	size = polynomial.size;
	
	head = polynomial.head;

	polynomial.head = NULL;
}
////////////////////////////////////////////////////////
//		Деструктор
////////////////////////////////////////////////////////
void Polynomial::Create()
{
	head = new ListItem(0);
	head->SetNext(head);
	head->SetPrev(head);
	size = 0;
}

void Polynomial::Destruct()
{
	ListItem* i = head->GetNext();
	while (i != head)
	{
		ListItem* pi = i;
		i = i->GetNext();
		delete pi;
	}
	delete head;
	head = 0;
	size = 0;
}

Polynomial::~Polynomial()
{
}

////////////////////////////////////////////////////////
//		Операторы
////////////////////////////////////////////////////////
Polynomial& Polynomial::operator=(const Polynomial &polynomial)
{
	Destruct();
	Create();

	ListItem* item = polynomial.First();

	for (int i = 0; i < polynomial.size; ++i)
	{
		Insert(item->GetVal());
		item = item->GetNext();
	}

	return *this;
}

Polynomial& Polynomial::operator=(Polynomial &&polynomial)
{
	Destruct();

	size = polynomial.size;

	head = polynomial.head;

	polynomial.head = NULL;

	return *this;
}


Polynomial& Polynomial::operator+=(const Polynomial &polynomial)
{
	if (size == 0 && polynomial.size == 0)
		return *this;

	while(size < polynomial.size)
	{
		InsertUp(0);
	}

	ListItem* item_other = polynomial.First();
	ListItem* item = First();
		
	for (int i = 0; i < size - polynomial.size; ++i)
		item = item->GetNext();

	for (int i = 0; i < polynomial.size; ++i)
	{
		item->SetVal(item->GetVal() + item_other->GetVal());

		item = item->GetNext();
		item_other = item_other->GetNext();//??
	}

	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial &polynomial)
{
	if (size == 0 && polynomial.size == 0)
		return *this;

	while (size < polynomial.size)
	{
		InsertUp(0);
	}

	ListItem* item_other = polynomial.First();
	ListItem* item = First();

	for (int i = 0; i < size - polynomial.size; ++i)
		item = item->GetNext();

	for (int i = 0; i < polynomial.size; ++i)
	{
		item->SetVal(item->GetVal() - item_other->GetVal());

		item = item->GetNext();
		item_other = item_other->GetNext();
	}

	return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial &polynomial)
{
	(*this) = karatsuba_mul(polynomial);

	return *this;
}

Polynomial& Polynomial::operator/=(Polynomial &polynomial)
{
	if (polynomial.size == 0)
	{
		throw PolynomialExeption("Empty Polynomial");
	}

	Normal();
	polynomial.Normal();

	if (size < polynomial.size)
	{
		Destruct();
		Create();
		Insert(0);

		return *this;
	}

	Polynomial res;
	Polynomial minus;
	int zeros = size - polynomial.size;

	

	while (size >= polynomial.size)
	{
		double m = (*this)[0] / polynomial[0];

		minus = polynomial * m;

		minus.SizeToEnd(minus.size + zeros);

		//std::cout << "(!!!)minus: " << minus << std::endl;

		operator-=(minus);

		Normal();

		--zeros;

		res.Insert(m);
	}

	(*this) = res;

	return *this;
}

Polynomial& Polynomial::operator%=(const Polynomial &polynomial)
{
	if (polynomial.size == 0)
	{
		throw PolynomialExeption("Empty Polynomial");
	}
	///polynomial[0]==0

	if (size < polynomial.size)
	{
		Destruct();
		Create();
		Insert(0);

		return *this;
	}

	Polynomial res;
	Polynomial minus;
	int zeros = size - polynomial.size;

	while (size >= polynomial.size)
	{
		double m = (*this)[0] / polynomial[0];

		minus = polynomial * m;

		minus.SizeToEnd(minus.size + zeros);

		std::cout << "(!!!)minus: " << minus << std::endl;

		operator-=(minus);

		Normal();

		--zeros;
	}

	return *this;
}

Polynomial& Polynomial::operator*=(double n)
{
	ListItem* item = First();

	for (int i = 0; i < size; ++i)
	{
		item->SetVal(item->GetVal() * n);

		item = item->GetNext();
	}

	return *this;
}

Polynomial& Polynomial::operator/=(double n)
{
	if (abs(n) < 0.00000000001)
	{
		throw PolynomialExeption("Division by zero error");
	}

	ListItem* item = First();

	for (int i = 0; i < size; ++i)
	{
		item->SetVal(item->GetVal() / n);

		item = item->GetNext();
	}

	return *this;
}



std::ostream& operator<<(std::ostream &os, const Polynomial &polynomial)
{
	os << "Polynomial: ";

	ListItem* item = polynomial.First();
	int size = polynomial.Size();

	for (int i = 0; i < size; ++i)
	{
		if (item->GetVal() == 0)
		{
			item = item->GetNext();
		}
		else if (item->GetVal() > 0)
		{
			std::cout << "+" << item->GetVal() << "x^" << size - i - 1;
			item = item->GetNext();
		}
		else
		{
			std::cout << item->GetVal() << "x^" << size - i - 1;
			item = item->GetNext();
		}
	}

#ifdef DEBUG
	os << std::endl << "List(" << size << "): ";

	item = polynomial.First();
	size = polynomial.Size();

	for (double i = 0; i < size; ++i)
	{
		std::cout << item->GetVal() << " ";
		item = item->GetNext();
	}
#endif
	return os;
}

std::istream& operator>>(std::istream &is, Polynomial &polynomial)
{
	char buff[BUFSIZ];

	std::cin >> buff;

	polynomial = Polynomial(buff);

	return is;
}

////////////////////////////////////////////////////////
//		Операторы (не компонентные)
////////////////////////////////////////////////////////
Polynomial operator+(const Polynomial &left, const Polynomial &right)
{
	Polynomial tmp(left);

	tmp += right;
	
	return tmp;
}

Polynomial operator-(const Polynomial &left, const Polynomial &right)
{
	Polynomial tmp(left);

	tmp -= right;

	return tmp;
}

Polynomial operator*(const Polynomial &left, const Polynomial &right)
{
	return left.karatsuba_mul(right);
}

Polynomial operator/(const Polynomial &left, Polynomial &right)
{
	Polynomial tmp(left);

	tmp /= right;

	return tmp;
}

Polynomial operator%(const Polynomial &left, const Polynomial &right)
{
	Polynomial tmp(left);

	tmp %= right;

	return tmp;
}


Polynomial operator*(const Polynomial &left, double n)
{
	Polynomial tmp(left);

	tmp *= n;

	return tmp;
}

Polynomial operator/(const Polynomial &left, double n)
{
	Polynomial tmp(left);

	tmp /= n;

	return tmp;
}

////////////////////////////////////////////////////////
//		Функции
////////////////////////////////////////////////////////
void Polynomial::SizeTo2()
{
	if (size & 1)
	{
		InsertUp(0);
	}
}

void Polynomial::SizeToUp(int n)
{
	while (size < n)
	{
		InsertUp(0);
	}
}

void Polynomial::SizeToEnd(int n)
{
	while (size < n)
	{
		Insert(0);
	}
}

void Polynomial::Normal()
{
	ListItem* item = First();

	int n = size - 1;
	for (int i = 0; i < n; ++i)
	{
		if (item->GetVal() != 0)
			break;

		item = item->GetNext();
		DeleteUp();
	}
}

Polynomial Polynomial::karatsuba_mul(const Polynomial &other) const
{
	if (size == 0 || other.size == 0)
	{
		throw PolynomialExeption("Empty Polynomial");
	}
	
	Polynomial res;

	if (size == 1 && other.size == 1)
	{
		res.Insert((*this)[0] * other[0]);
		return res;
	}


	Polynomial X(*this);
	Polynomial Y(other);

	//std::cout << std::endl << "X: " << X << std::endl;
	//std::cout << "Y: " << Y << std::endl;

	X.SizeTo2();
	Y.SizeTo2();

	//std::cout << std::endl << "X: " << X << std::endl;
	//std::cout << "Y: " << Y << std::endl;

	const int n = std::max(X.size, Y.size);

	X.SizeToUp(n);
	Y.SizeToUp(n);

	int k = n / 2;

	Polynomial Xl(X, 0, k);
	Polynomial Xr(X, k, INT_MAX);
	Polynomial Yl(Y, 0, k);
	Polynomial Yr(Y, k, INT_MAX);

	//std::cout << std::endl << "Xl: " << Xl << std::endl;
	//std::cout << "Xr: " << Xr << std::endl;

	//std::cout << "Yl: " << Yl << std::endl;
	//std::cout << "Yr: " << Yr << std::endl;

	Polynomial P1 = Xl.karatsuba_mul(Yl);
	Polynomial P2 = Xr.karatsuba_mul(Yr);
	Polynomial P3 = (Xr + Xl).karatsuba_mul(Yl + Yr);

	//std::cout << std::endl << "P1: " << P1 << std::endl;
	//std::cout << "P2: " << P2 << std::endl;
	//std::cout << "P3: " << P3 << std::endl;

	P3 -= P2;
	P3 -= P1;

	P1.SizeToEnd(P1.size + n);
	P3.SizeToEnd(P3.size + k);

	//std::cout << std::endl << "P1: " << P1 << std::endl;
	//std::cout << "P2: " << P2 << std::endl;
	//std::cout << "P3: " << P3 << std::endl;
	
	res = P1 + P2 + P3;

	//std::cout << std::endl << "res: " << res << std::endl;
	return res;
}

double Polynomial::Solve(double x)
{
	if (size <= 0)
	{
		throw PolynomialExeption("Empty Polynomial");
	}

	double res = 0;

	ListItem* item = Last()->GetPrev();

	res += item->GetVal();

	item = item->GetPrev();

	for (int i = 1; i < size; ++i)
	{
		res += item->GetVal() * x;
		item = item->GetPrev();
		x *= x;
	}

	return res;
}

Polynomial Polynomial::Derivative()
{
	if (size <= 0)
	{
		throw PolynomialExeption("Empty Polynomial");
	}

	Polynomial tmp(*this);

	ListItem* item = tmp.Last()->GetPrev();

	for (int i = 0; i < size; ++i)
	{
		item->SetVal(item->GetVal() * i);
		item = item->GetPrev();
	}

	tmp.DeleteEnd();

	return tmp;
}