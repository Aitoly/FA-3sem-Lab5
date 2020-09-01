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

Polynomial::Polynomial(const char* str)
{
	char *in = new char[strlen(str) + 1];
	strcpy(in, str);
	char* current = in;

	try
	{
		/*head = new ListForPolynomialItem();
		head->SetNext(head);
		head->SetPrev(head);
		size = 0;*/

		char* next = current;

		while (*current != '\0' && *current != '\n')
		{
			while (*next != '\0' && *next != '\n' && *next != '+' && *next != '-')
			{
				++next;
			}

			if (*current == '!')
			{
				*current = '+';
			}
			if (*current == '?')
			{
				*current = '-';
			}


			if (*next == '+' || *next == '!')
			{
				*next = 0;
				ListForPolynomialItem tmp(current);
				current = next;
				Insert(tmp);
				*next = '!';
			}
			else if (*next == '-' || *next == '?')
			{
				*next = 0;
				ListForPolynomialItem tmp(current);
				current = next;
				Insert(tmp);
				*next = '?';
			}
			else
			{
				ListForPolynomialItem tmp(current);
				current = next;
				Insert(tmp);
			}
		}

		delete[] in;
		
	}
	catch (PolynomialExeption &Ex)
	{
		std::cout << Ex.what() << std::endl;
		head = new ListForPolynomialItem();
		head->SetNext(head);
		head->SetPrev(head);
		size = 0;

		delete[] in;
	}
}

Polynomial::Polynomial(const Polynomial &polynomial)
{
	ListForPolynomialItem* item = polynomial.First();

	for (int i = 0; i < polynomial.size; ++i)
	{
		Insert(*item);
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
	head = new ListForPolynomialItem();
	head->SetNext(head);
	head->SetPrev(head);
	size = 0;
}

void Polynomial::Destruct()
{
	ListForPolynomialItem* i = head->GetNext();
	while (i != head)
	{
		ListForPolynomialItem* pi = i;
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
	if (&polynomial == this)
	{
		return *this;
	}

	Destruct();
	Create();

	ListForPolynomialItem* item = polynomial.First();

	for (int i = 0; i < polynomial.size; ++i)
	{
		Insert(*item);
		item = item->GetNext();
	}

	return *this;
}

Polynomial& Polynomial::operator=(Polynomial &&polynomial)
{
	if (&polynomial == this)
	{
		return *this;
	}

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

	if (&polynomial == this)
	{
		//return *this;
	}

	ListForPolynomialItem* item = polynomial.First();

	for (int i = 0; i < polynomial.size; ++i)
	{
		Insert(*item);
		item = item->GetNext();
	}

	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial &polynomial)
{
	if (size == 0 && polynomial.size == 0)
		return *this;

	if (&polynomial == this)
	{
		//return *this; TODO
	}

	ListForPolynomialItem* item = polynomial.First();

	for (int i = 0; i < polynomial.size; ++i)
	{
		Insert(*item, -1);
		item = item->GetNext();
	}

	return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial &polynomial)
{
	if (size == 0 && polynomial.size == 0)
		return *this;

	Polynomial tmp;

	ListForPolynomialItem* item = polynomial.First();
	int n = polynomial.size;

	for (int i = 0; i < n; ++i)
	{
		Polynomial tmp_ = (*this) * (*item);

		ListForPolynomialItem* item_tmp = tmp_.First();

		for (int j = 0; j < tmp_.size; ++j)
		{
			tmp.Insert(*item_tmp);
			item_tmp = item_tmp->GetNext();
		}

		item = item->GetNext();
	}

	(*this) = tmp;

	return *this;
}

bool Polynomial::operator==(const Polynomial &polynomial)
{
	if (size == 0 && polynomial.size == 0)
	{
		return true;
	}
	if (size == 0)
	{
		return false;
	}
	if (polynomial.size == 0)
	{
		return false;
	}
	if (size != polynomial.size)
	{
		return false;
	}



	ListForPolynomialItem* item = First();
	ListForPolynomialItem* item_other = polynomial.First();

	for (int i = 0; i < size; ++i)
	{
		if (*item != *item_other)
		{
			return false;
		}

		item = item->GetNext();
		item_other = item_other->GetNext();
	}

	return true;
}

bool Polynomial::operator!=(const Polynomial &polynomial)
{
	return !operator==(polynomial);
}


std::ostream& operator<<(std::ostream &os, const Polynomial &polynomial)
{
	os << "Polynomial: ";

	if (polynomial.size == 0)
	{
		os << 0;
		return os;
	}

	ListForPolynomialItem* item = polynomial.First();
	int size = polynomial.Size();

	for (int i = 0; i < size; ++i)
	{
		/*if (item->GetKoeff() == 0)
		{
			item = item->GetNext();
		}
		else */if (item->GetKoeff() > 0)
		{
			std::cout << "+" << *item;
			item = item->GetNext();
		}
		else
		{
			std::cout << *item;
			item = item->GetNext();
		}
	}
	return os;
}

std::istream& operator>>(std::istream &is, Polynomial &polynomial)
{
	char buff[BUFSIZ];

	is >> buff;

	polynomial = Polynomial(buff);

	return is;
}

////////////////////////////////////////////////////////
//		Операторы (не компонентные)
////////////////////////////////////////////////////////
Polynomial operator+(const Polynomial &left, const Polynomial &right)
{
	std::cout << "left" << left << std::endl;
	Polynomial tmp(left);
	std::cout << "tmp" << tmp << std::endl;
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
	Polynomial tmp(left);

	tmp *= right;

	return tmp;
}

Polynomial operator*(const Polynomial &left, const ListForPolynomialItem &item_)
{
	Polynomial tmp(left);

	ListForPolynomialItem* item = tmp.First();

	int n = tmp.size;

	for (int i = 0; i < n; ++i)
	{
		*item *= item_;
		item = item->GetNext();
	}

	return tmp;
}
