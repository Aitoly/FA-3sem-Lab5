#pragma warning(disable:4996)
#include<iostream>
#include<algorithm>

#include"ListForMonomial.h"

#include "Monomial.h"


////////////////////////////////////////////////////////
//		»ÒÍÎ˛˜ÂÌËˇ
////////////////////////////////////////////////////////

MonomialExÒeption::MonomialExÒeption()
:error(NULL)
{}

MonomialExÒeption::MonomialExÒeption(const char* error_)
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

MonomialExÒeption::MonomialExÒeption(const MonomialExÒeption& Ex)
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

MonomialExÒeption::~MonomialExÒeption()
{
	if (error)
		delete[] error;
}

const char* MonomialExÒeption::what()
{
	return error;
}







////////////////////////////////////////////////////////
//		 ŒÕ—“–” “Œ–€
////////////////////////////////////////////////////////

Monomial::Monomial() : ListForMonomial()
{
	koeff = 0;
}

Monomial::Monomial(char* str) : ListForMonomial()
{
	try
	{
		koeff = 0;
		int len = strlen(str);
		bool plus = true;
		bool firstFinded = false;
		int power = 0;
		char name = '#';


		/*head = new ListForMonomialItem(0);
		head->SetNext(head);
		head->SetPrev(head);
		size = 0;*/

		if (*str == '-')
		{
			plus = false;
			++str;
		}
		if (*str == '+')
		{
			++str;
		}



		if (isalpha(*str))
		{
			koeff = 1;
		}
		else
		{
			while (!isalpha(*str) && *str != '\0' && *str != '\n' && *str != '+' && *str != '-')
			{
				if (isspace(*str))
				{
					++str;
					continue;
				}

				if (isdigit(*str))
				{
					koeff *= 10;
					koeff += *str - '0';
					++str;
				}
				else
				{
					throw MonomialExÒeption("Unexpected symbol");
				}
			}
		}



		if (!plus)
		{
			koeff *= -1;
		}



		while (*str != '\0' && *str != '\n' && *str != '+' && *str != '-')
		{
			if (isspace(*str) || *str == '^')
			{
				++str;
				continue;
			}
			else if (isdigit(*str))
			{
				power *= 10;
				power += *str - '0';
				++str;
			}
			else if (isalpha(*str))
			{
				if (power != 0 && name != '#')
				{
					Insert(name, power);
					power = 0;
					name = '#';
				}
				else
				{
					power = 0;
					name = '#';
				}
				name = *str;
				++str;
			}
			else
			{
				throw MonomialExÒeption("Unexpected symbol");
			}



			if (power != 0 && name != '#')
			{
				Insert(name, power);
				power = 0;
				name = '#';
			}
		}
	}
	catch (MonomialExÒeption &Ex)
	{
		std::cout << Ex.what() << std::endl;
		head = new ListForMonomialItem('#', 0);
		head->SetNext(head);
		head->SetPrev(head);
		size = 0;
	}
}

Monomial::Monomial(const Monomial &other) : ListForMonomial(other)
{
	koeff = other.koeff;
}

Monomial::Monomial(Monomial &&other) : ListForMonomial(other)
{
	koeff = other.koeff;
}

////////////////////////////////////////////////////////
//		ƒ≈—“–” “Œ–
////////////////////////////////////////////////////////
Monomial::~Monomial()
{

}

////////////////////////////////////////////////////////
//		Œœ≈–¿“Œ–€
////////////////////////////////////////////////////////
Monomial& Monomial::operator=(const Monomial &other)
{
	Destroy();
	Create();

	ListForMonomialItem* item = other.First();

	for (int i = 0; i < other.size; ++i)
	{
		Insert(item->GetName(), item->GetPower());
		item = item->GetNext();
	}

	koeff = other.koeff;

	return *this;
}

Monomial& Monomial::operator=(Monomial &&other)
{
	Destroy();
	Create();

	size = other.size;
	other.size = 0;

	head = other.head;
	other.head = NULL;

	koeff = other.koeff;

	return *this;
}

bool Monomial::operator<(const Monomial &other)
{
	if (size == 0 && other.size == 0)
	{
		return false;
	}
	if (size == 0)
	{
		return true;
	}
	if (other.size == 0)
	{
		return false;
	}

	int n = std::max(size, other.size);

	ListForMonomialItem* item = First();
	ListForMonomialItem* item_other = other.First();

	for (int i = 0; i < n; ++i)
	{
		if (item->GetName() < item_other->GetName())
		{
			return true;
		}
		else if (item->GetName() > item_other->GetName())
		{
			return false;
		}

		if (item->GetPower() < item_other->GetPower())
		{
			return true;
		}
		else if (item->GetPower() > item_other->GetPower())
		{
			return false;
		}

		item = item->GetNext();
		item_other = item_other->GetNext();
	}

	return false;
}

bool Monomial::operator==(const Monomial &other)
{
	if (size == 0 && other.size == 0)
	{
		return true;
	}
	if (size == 0)
	{
		return false;
	}
	if (other.size == 0)
	{
		return false;
	}
	if (size != other.size)
	{
		return false;
	}

	ListForMonomialItem* item = First();
	ListForMonomialItem* item_other = other.First();

	for (int i = 0; i < size; ++i)
	{
		if (item->GetName() != item_other->GetName())
		{
			return false;
		}

		if (item->GetPower() != item_other->GetPower())
		{
			return false;
		}

		item = item->GetNext();
		item_other = item_other->GetNext();
	}

	return true;
}

bool Monomial::operator>(const Monomial &other)
{
	return !operator<(other) && !operator==(other);
}

bool Monomial::operator!=(const Monomial &other)
{
	return !operator==(other);
}

Monomial& Monomial::operator*=(const Monomial &other)
{
	if (&other == this)
	{
		ListForMonomialItem* item_f = First();
		for (int i = 0; i < size; ++i)
		{
			item_f->SetPower(item_f->GetPower() * 2);
			item_f = item_f->GetNext();
		}
		koeff *= koeff;
		return *this;
	}

	ListForMonomialItem* item_f = First();
	ListForMonomialItem* item_l = Last();
	ListForMonomialItem* item_other = other.First();

	koeff *= other.koeff;

	for (int i = 0; i < other.size; ++i)
	{
		while (true)
		{

			if (item_f == item_l)
			{
				Insert(item_other->GetName(), item_other->GetPower());
				break;
			}
			if (item_f->GetName() == item_other->GetName())
			{
				(*item_f) *= (*item_other);
				if (item_f->GetPower() == 0)
				{
					Delete(item_f);
				}
				item_f = item_f->GetNext();
				break;
			}
			if (item_f->GetName() > item_other->GetName() && item_f->GetName() != item_other->GetName())
			{
				Insert(item_other->GetName(), item_other->GetPower());
				break;
			}
			item_f = item_f->GetNext();
		}

		item_other = item_other->GetNext();
	}

	return *this;
}

Monomial& Monomial::operator/=(const Monomial &other)
{
	if (&other == this)
	{
		Destroy();
		Create();
		koeff = 1;
		return *this;
	}

	ListForMonomialItem* item_f = First();
	ListForMonomialItem* item_l = Last();
	ListForMonomialItem* item_other = other.First();

	koeff /= other.koeff;

	for (int i = 0; i < other.size; ++i)
	{
		while (true)
		{
			if (item_f == item_l)
			{
				Insert(item_other->GetName(), -1 * item_other->GetPower());
				break;
			}
			else if (item_f->GetName() == item_other->GetName())
			{
				(*item_f) /= (*item_other);
				if (item_f->GetPower() == 0)
				{
					item_f = item_f->GetPrev();
					Delete(item_f->GetNext());
				}
				item_f = item_f->GetNext();
				break;
			}
			else if (item_f->GetName() > item_other->GetName() && item_f->GetName() != item_other->GetName())
			{
				Insert(item_other->GetName(), -1 * item_other->GetPower());
				break;
			}
			item_f = item_f->GetNext();
		}

		item_other = item_other->GetNext();
	}

	return *this;
}

std::ostream& operator<<(std::ostream &os, const Monomial &monomial)
{
	os << monomial.koeff;

	if (monomial.size == 0)
	{
		return os;
	}

	
	ListForMonomialItem* item = monomial.First();

	for (int i = 0; i < monomial.size; ++i)
	{
		os << *item;
		item = item->GetNext();
	}

	return os;
}

std::istream& operator>>(std::istream &is, Monomial &monomial)
{
	static char buff[BUFSIZ];
	is >> buff;
	monomial = Monomial(buff);
	return is;
}



Monomial operator*(const Monomial &left, const Monomial &right)
{
	Monomial tmp(left);
	tmp *= right;
	return tmp;
}

Monomial operator/(const Monomial &left, const Monomial &right)
{
	Monomial tmp(left);
	tmp /= right;
	return tmp;
}

////////////////////////////////////////////////////////
//		‘”Õ ÷»»
////////////////////////////////////////////////////////

double Monomial::GetKoeff() const
{
	return koeff;
}

void Monomial::SetKoeff(double n)
{
	koeff = n;
}