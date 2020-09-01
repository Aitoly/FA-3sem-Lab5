#pragma once
#include"Monomial.h"


class ListForPolynomialException
{
private:
	char* error;
	ListForPolynomialException();


public:
	ListForPolynomialException(const char*);
	ListForPolynomialException(const ListForPolynomialException&);
	~ListForPolynomialException();

	const char* what();
};






class ListForPolynomialItem : public Monomial
{
private:
	ListForPolynomialItem* prev;
	ListForPolynomialItem* next;
	
	
	
	ListForPolynomialItem& operator=(ListForPolynomialItem&&);
	ListForPolynomialItem& operator=(const ListForPolynomialItem&);

public:
	ListForPolynomialItem();
	ListForPolynomialItem(char*);
	ListForPolynomialItem(const ListForPolynomialItem&);
	ListForPolynomialItem(ListForPolynomialItem&&);

	~ListForPolynomialItem();

	ListForPolynomialItem* GetPrev() const;
	ListForPolynomialItem* GetNext() const;
	void SetPrev(ListForPolynomialItem*);
	void SetNext(ListForPolynomialItem*);
};







class ListForPolynomial
{
protected:
	ListForPolynomialItem* head;
	int size;
	ListForPolynomialItem* First() const;
	ListForPolynomialItem* Last() const;
	ListForPolynomial();
	ListForPolynomial(const ListForPolynomial&);
	ListForPolynomial(const ListForPolynomial&&);

public:
	
	~ListForPolynomial();

	int Size() const;
	void Insert(ListForPolynomialItem&, int = 1);
	void Delete(int);
	void Delete(ListForPolynomialItem*);

	ListForPolynomialItem& operator[](int) const;
};

