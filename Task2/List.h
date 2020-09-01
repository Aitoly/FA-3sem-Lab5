#pragma once

class ListExeption
{
private:
	char* error;
	ListExeption();


public:
	ListExeption(const char*);
	ListExeption(const ListExeption&);
	~ListExeption();

	const char* what();
};












class ListItem
{
private:
	double data;
	ListItem* prev;
	ListItem* next;
	
	ListItem();
	ListItem(ListItem&);
	ListItem(ListItem&&);
	
	ListItem& operator=(ListItem&&);
	ListItem& operator=(const ListItem&);

public:
	explicit ListItem(double);
	~ListItem();

	
	ListItem& operator=(double);

	ListItem& operator+=(const ListItem&);
	ListItem& operator-=(const ListItem&);
	ListItem& operator*=(const ListItem&);
	ListItem& operator/=(const ListItem&);

	ListItem& operator+=(double);
	ListItem& operator-=(double);
	ListItem& operator*=(double);
	ListItem& operator/=(double);

	ListItem* GetPrev() const;
	ListItem* GetNext() const;
	void SetPrev(ListItem*);
	void SetNext(ListItem*);
	double GetVal() const;

	
	void SetVal(double);
};

double operator+(const ListItem&, const ListItem&);
double operator-(const ListItem&, const ListItem&);
double operator*(const ListItem&, const ListItem&);
double operator/(const ListItem&, const ListItem&);

std::ostream& operator<<(std::ostream&, const ListItem&);













class List
{
protected:
	ListItem* head;
	int size;
	ListItem* First() const;
	ListItem* Last() const;
	List();
	List(const List&);
	List(const List&&);

public:
	
	~List();

	int Size() const;
	void Insert(const double);
	void InsertUp(const double);
	void DeleteUp();
	void DeleteEnd();

	ListItem& operator[](int) const;

	friend std::ostream& operator<<(std::ostream&, const List&);
};

