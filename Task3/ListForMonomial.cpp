#pragma warning(disable:4996)
#include<iostream>

#include "ListForMonomial.h"

////////////////////////////////////////////////////////
//		Исключения
////////////////////////////////////////////////////////

ListForMonomialException::ListForMonomialException()
:error(NULL)
{}

ListForMonomialException::ListForMonomialException(const char* error_)
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

ListForMonomialException::ListForMonomialException(const ListForMonomialException& Ex)
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

ListForMonomialException::~ListForMonomialException()
{
	if (error)
		delete[] error;
}

const char* ListForMonomialException::what()
{
	return error;
}



////////////////////////////////////////////////////////
//		LIST ITEM
////////////////////////////////////////////////////////
ListForMonomialItem::ListForMonomialItem()
{

}

ListForMonomialItem::ListForMonomialItem(const ListForMonomialItem &other)
{
	name = other.name;
	power = other.power;
}


ListForMonomialItem::ListForMonomialItem(ListForMonomialItem &&other)
{
	name = other.name;
	power = other.power;
}

ListForMonomialItem::ListForMonomialItem(char name_, int power_)
{
	name = name_;
	power = power_;
	prev = nullptr;
	next = nullptr;
}

ListForMonomialItem::~ListForMonomialItem()
{

}

ListForMonomialItem* ListForMonomialItem::GetPrev() const
{
	return prev;
}

ListForMonomialItem* ListForMonomialItem::GetNext() const
{
	return next;
}

char ListForMonomialItem::GetName() const
{
	return name;
}

int ListForMonomialItem::GetPower() const
{
	return power;
}


void ListForMonomialItem::SetPrev(ListForMonomialItem* L_I)
{
	if (L_I)
		prev = L_I;
}

void ListForMonomialItem::SetNext(ListForMonomialItem* L_I)
{
	if (L_I)
		next = L_I;
}

void ListForMonomialItem::SetName(char name_)
{
	name = name_;
}

void ListForMonomialItem::SetPower(int power_)
{
	power = power_;
}


ListForMonomialItem& ListForMonomialItem::operator*=(const ListForMonomialItem &item)
{
	SetPower(GetPower() + item.GetPower());
	return *this;
}

ListForMonomialItem& ListForMonomialItem::operator/=(const ListForMonomialItem &item)
{
	SetPower(GetPower() - item.GetPower());
	return *this;
}

std::ostream& operator<<(std::ostream &os, const ListForMonomialItem& item)
{
	os << item.GetName() << "^" << item.GetPower();
	return os;
}

std::istream& operator>>(std::istream &is, ListForMonomialItem &item)
{
	char name;
	int power;
	is >> name;
	is >> power;
	item = ListForMonomialItem(name, power);
	return is;
}

ListForMonomialItem& ListForMonomialItem::operator=(ListForMonomialItem &&other)
{
	name = other.name;
	power = other.power;

	return *this;
}

ListForMonomialItem& ListForMonomialItem::operator=(const ListForMonomialItem &other)
{
	name = other.name;
	power = other.power;
	
	return *this;
}





////////////////////////////////////////////////////////
///			LIST
////////////////////////////////////////////////////////
ListForMonomial::ListForMonomial()
{
	head = new ListForMonomialItem('#', 0);
	head->SetNext(head);
	head->SetPrev(head);
	size = 0;
}

ListForMonomial::ListForMonomial(const ListForMonomial &other)
{
	Create();
	ListForMonomialItem* item = other.First();

	for (int i = 0; i < other.size; ++i)
	{
		Insert(item->GetName(), item->GetPower());
		item = item->GetNext();
	}
}

ListForMonomial::ListForMonomial(ListForMonomial &&other)
{
	size = other.size;
	other.size = 0;

	head = other.head;
	other.head = NULL;
}

void ListForMonomial::Create()
{
	head = new ListForMonomialItem('#', 0);
	head->SetNext(head);
	head->SetPrev(head);
	size = 0;
}

ListForMonomial::~ListForMonomial()
{
	if (head)
	{
		ListForMonomialItem* i = head->GetNext();
		while (i != head)
		{
			ListForMonomialItem* pi = i;
			i = i->GetNext();
			delete pi;
		}
		delete head;
		head = 0;
		size = 0;
	}
}

void ListForMonomial::Destroy()
{
	if (head)
	{
		ListForMonomialItem* i = head->GetNext();
		while (i != head)
		{
			ListForMonomialItem* pi = i;
			i = i->GetNext();
			delete pi;
		}
		delete head;
		head = 0;
		size = 0;
	}
}

int ListForMonomial::Size() const
{
	return size;
}

void ListForMonomial::Insert(char name_, int power_)
{
	ListForMonomialItem* current = First();
	ListForMonomialItem* last = Last();
	ListForMonomialItem* res = new ListForMonomialItem(name_, power_);
	
	while (current->GetName() < name_ && current != last)
	{
		current = current->GetNext();
	}

	res->SetNext(current);
	res->SetPrev(current->GetPrev());
	ListForMonomialItem* prev = res->GetPrev();
	prev->SetNext(res);
	current->SetPrev(res);

	size++;
}

void ListForMonomial::Delete(int i)
{
	if (size <= 0)
		return;

	ListForMonomialItem* current = &operator[](i);
	ListForMonomialItem* res = Last();

	res = current->GetNext();
	res->SetPrev(current->GetPrev());
	ListForMonomialItem* prev = current->GetPrev();
	prev->SetNext(res);
	size--;
	free(current);
	current = NULL;
}

void ListForMonomial::Delete(ListForMonomialItem *item)
{
	if (size <= 0)
		return;

	ListForMonomialItem* res = Last();

	res = item->GetNext();
	res->SetPrev(item->GetPrev());
	ListForMonomialItem* prev = item->GetPrev();
	prev->SetNext(res);
	size--;
	free(item);
	item = NULL;
}

ListForMonomialItem* ListForMonomial::First() const
{
	return head->GetNext();
}

ListForMonomialItem* ListForMonomial::Last() const
{
	return head;
}

ListForMonomialItem& ListForMonomial::operator[](int n) const
{
	if (n < 0 || n >= size)
		throw ListForMonomialException("Out of range []");

	ListForMonomialItem* item = First();

	for (int i = 0; i < n; ++i)
	{
		item = item->GetNext();
	}

	return *item;
}

std::ostream& operator<<(std::ostream &os, const ListForMonomial &List)
{
	ListForMonomialItem* item = List.First();

	for (int i = 0; i < List.size; ++i)
	{
		os << item;
		item = item->GetNext();
	}

	return os;
}
