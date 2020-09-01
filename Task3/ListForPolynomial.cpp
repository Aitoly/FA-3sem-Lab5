#pragma warning(disable:4996)
#include<iostream>
#include<algorithm>

#include"Monomial.h"
#include "ListForPolynomial.h"

////////////////////////////////////////////////////////
//		Исключения
////////////////////////////////////////////////////////

ListForPolynomialException::ListForPolynomialException()
:error(NULL)
{}

ListForPolynomialException::ListForPolynomialException(const char* error_)
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

ListForPolynomialException::ListForPolynomialException(const ListForPolynomialException& Ex)
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

ListForPolynomialException::~ListForPolynomialException()
{
	if (error)
		delete[] error;
}

const char* ListForPolynomialException::what()
{
	return error;
}



////////////////////////////////////////////////////////
//		LIST ITEM
////////////////////////////////////////////////////////
ListForPolynomialItem::ListForPolynomialItem() : Monomial()
{
	prev = nullptr;
	next = nullptr;
}

ListForPolynomialItem::ListForPolynomialItem(char* str) : Monomial(str)
{
	prev = nullptr;
	next = nullptr;
}

ListForPolynomialItem::ListForPolynomialItem(const ListForPolynomialItem &other) : Monomial(other)
{
	prev = nullptr;
	next = nullptr;
}

ListForPolynomialItem::ListForPolynomialItem(ListForPolynomialItem &&other) : Monomial(other)
{
	prev = nullptr;
	next = nullptr;
}

ListForPolynomialItem::~ListForPolynomialItem()
{

}

ListForPolynomialItem* ListForPolynomialItem::GetPrev() const
{
	return prev;
}

ListForPolynomialItem* ListForPolynomialItem::GetNext() const
{
	return next;
}

void ListForPolynomialItem::SetPrev(ListForPolynomialItem* L_I)
{
	if (L_I)
		prev = L_I;
}

void ListForPolynomialItem::SetNext(ListForPolynomialItem* L_I)
{
	if (L_I)
		next = L_I;
}

/*ListForPolynomialItem& ListForPolynomialItem::operator*=(const ListForPolynomialItem &item)
{
	SetVal(GetVal() * item.GetVal());
	return *this;
}

ListForPolynomialItem& ListForPolynomialItem::operator/=(const ListForPolynomialItem &item)
{
	if (item.GetVal() == 0)
	{
		throw ListForPolynomialException("Division by zero error");
	}

	SetVal(GetVal() / item.GetVal());
	return *this;
}

std::ostream& operator<<(std::ostream &os, const ListForPolynomialItem& item)
{
	os << item.GetVal();

	return os;
}

double operator*(const ListForPolynomialItem &left, const ListForPolynomialItem &right)
{
	return left.GetVal() * right.GetVal();
}

double operator/(const ListForPolynomialItem &left, const ListForPolynomialItem &right)
{
	if (abs(right.GetVal()) < 0.0000000001)
	{
		throw ListForPolynomialException("Division by zero error");
	}

	return left.GetVal() / right.GetVal();
}
*/







////////////////////////////////////////////////////////
///			LIST
////////////////////////////////////////////////////////
ListForPolynomial::ListForPolynomial()
{
	head = new ListForPolynomialItem();
	head->SetNext(head);
	head->SetPrev(head);
	size = 0;
}

ListForPolynomial::~ListForPolynomial()
{
	if (head)
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
}

int ListForPolynomial::Size() const
{
	return size;
}

void ListForPolynomial::Insert(ListForPolynomialItem &item, int m)
{
	
	ListForPolynomialItem* current = First();
	ListForPolynomialItem* last = Last();
	ListForPolynomialItem* res = new ListForPolynomialItem(item);
	
	if (size != 0)
	{
		while ((*current < item) && current != last)
		{
			current = current->GetNext();
		}

		if (*current == item)
		{
			current->SetKoeff(current->GetKoeff() + (m * item.GetKoeff()));

			if (abs(current->GetKoeff()) < 0.00000000001)
			{
				Delete(current);
			}
			return;
		}

	}

	res->SetKoeff(res->GetKoeff() * m);

	res->SetNext(current);
	res->SetPrev(current->GetPrev());
	ListForPolynomialItem* prev = res->GetPrev();
	prev->SetNext(res);
	current->SetPrev(res);

	size++;
}

void ListForPolynomial::Delete(int i)
{
	if (size <= 0)
		return;

	ListForPolynomialItem* current = &operator[](i);
	ListForPolynomialItem* res = Last();

	res = current->GetNext();
	res->SetPrev(current->GetPrev());
	ListForPolynomialItem* prev = current->GetPrev();
	prev->SetNext(res);
	size--;
	free(current);
	current = NULL;
}

void ListForPolynomial::Delete(ListForPolynomialItem *item)
{
	if (size <= 0)
		return;

	ListForPolynomialItem* res = Last();

	res = item->GetNext();
	res->SetPrev(item->GetPrev());
	ListForPolynomialItem* prev = item->GetPrev();
	prev->SetNext(res);
	size--;
	free(item);
	item = NULL;
}

ListForPolynomialItem* ListForPolynomial::First() const
{
	return head->GetNext();
}

ListForPolynomialItem* ListForPolynomial::Last() const
{
	return head;
}

ListForPolynomialItem& ListForPolynomial::operator[](int n) const
{
	if (n < 0 || n >= size)
		throw ListForPolynomialException("Out of range []");

	ListForPolynomialItem* item = First();

	for (int i = 0; i < n; ++i)
	{
		item = item->GetNext();
	}

	return *item;
}
