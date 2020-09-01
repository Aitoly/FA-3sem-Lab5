#pragma warning(disable:4996)
#include<iostream>

#include "List.h"

////////////////////////////////////////////////////////
//		Исключения
////////////////////////////////////////////////////////

ListExeption::ListExeption()
:error(NULL)
{}

ListExeption::ListExeption(const char* error_)
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

ListExeption::ListExeption(const ListExeption& Ex)
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

ListExeption::~ListExeption()
{
	if (error)
		delete[] error;
}

const char* ListExeption::what()
{
	return error;
}



////////////////////////////////////////////////////////
//		LIST ITEM
////////////////////////////////////////////////////////
ListItem::ListItem()
{

}

ListItem::ListItem(double x)
{
	data = x;
	prev = nullptr;
	next = nullptr;
}

ListItem::~ListItem()
{

}

ListItem* ListItem::GetPrev() const
{
	return prev;
}

ListItem* ListItem::GetNext() const
{
	return next;
}

double ListItem::GetVal() const
{
	return data;
}

void ListItem::SetPrev(ListItem* L_I)
{
	if (L_I)
		prev = L_I;
}

void ListItem::SetNext(ListItem* L_I)
{
	if (L_I)
		next = L_I;
}

void ListItem::SetVal(double x)
{
	data = x;
}


ListItem& ListItem::operator=(double n)
{
	SetVal(n);
	return *this;
}

ListItem& ListItem::operator+=(const ListItem &item)
{
	SetVal(GetVal() + item.GetVal());
	return *this;
}

ListItem& ListItem::operator-=(const ListItem &item)
{
	SetVal(GetVal() - item.GetVal());
	return *this;
}

ListItem& ListItem::operator*=(const ListItem &item)
{
	SetVal(GetVal() * item.GetVal());
	return *this;
}

ListItem& ListItem::operator/=(const ListItem &item)
{
	if (item.GetVal() == 0)
	{
		throw ListExeption("Division by zero error");
	}

	SetVal(GetVal() / item.GetVal());
	return *this;
}


ListItem& ListItem::operator+=(double n)
{
	SetVal(GetVal() + n);
	return *this;
}

ListItem& ListItem::operator-=(double n)
{
	SetVal(GetVal() - n);
	return *this;
}

ListItem& ListItem::operator*=(double n)
{
	SetVal(GetVal() * n);
	return *this;
}

ListItem& ListItem::operator/=(double n)
{
	if (abs(n) < 0.00000000001)
	{
		throw ListExeption("Division by zero error");
	}

	SetVal(GetVal() / n);
	return *this;
}


std::ostream& operator<<(std::ostream &os, const ListItem& item)
{
	os << item.GetVal();

	return os;
}

double operator+(const ListItem &left, const ListItem &right)
{
	return left.GetVal() + right.GetVal();
}

double operator-(const ListItem &left, const ListItem &right)
{
	return left.GetVal() - right.GetVal();
}

double operator*(const ListItem &left, const ListItem &right)
{
	return left.GetVal() * right.GetVal();
}

double operator/(const ListItem &left, const ListItem &right)
{
	if (abs(right.GetVal()) < 0.0000000001)
	{
		throw ListExeption("Division by zero error");
	}

	return left.GetVal() / right.GetVal();
}








////////////////////////////////////////////////////////
///			LIST
////////////////////////////////////////////////////////
List::List()
{
	head = new ListItem(0);
	head->SetNext(head);
	head->SetPrev(head);
	size = 0;
}

List::~List()
{
	if (head)
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
}

int List::Size() const
{
	return size;
}

void List::Insert(const double t)
{
	ListItem* i = Last();
	ListItem* res = new ListItem(t);

	res->SetNext(i);
	res->SetPrev(i->GetPrev());
	ListItem* prev = res->GetPrev();
	prev->SetNext(res);
	i->SetPrev(res);

	size++;
}

void List::InsertUp(const double t)
{
	ListItem* i = First();
	ListItem* res = new ListItem(t);

	res->SetNext(i);
	res->SetPrev(i->GetPrev());
	ListItem* prev = res->GetPrev();
	prev->SetNext(res);
	i->SetPrev(res);

	size++;
}

void List::DeleteUp()
{
	if (size <= 0)
		return;

	ListItem* i = First();
	ListItem* res = Last();

	res = i->GetNext();
	res->SetPrev(i->GetPrev());
	ListItem* prev = i->GetPrev();
	prev->SetNext(res);
	size--;
	free(i);
	i = 0;
}

void List::DeleteEnd()
{
	if (size <= 0)
		return;

	ListItem* i = Last()->GetPrev();
	ListItem* res = Last();

	res = i->GetNext();
	res->SetPrev(i->GetPrev());
	ListItem* prev = i->GetPrev();
	prev->SetNext(res);
	size--;
	free(i);
	i = 0;
}


ListItem* List::First() const
{
	return head->GetNext();
}

ListItem* List::Last() const
{
	return head;
}

ListItem& List::operator[](int n) const
{
	if (n < 0 || n >= size)
		throw ListExeption("Out of range []");

	ListItem* item = First();

	for (int i = 0; i < n; ++i)
	{
		item = item->GetNext();
	}

	return *item;
}

std::ostream& operator<<(std::ostream &os, const List &List)
{
	os << "List: ";

	ListItem* item = List.First();

	for (int i = 0; i < List.size; ++i)
	{
		std::cout << item->GetVal() << " ";
		item = item->GetNext();
	}

	return os;
}
