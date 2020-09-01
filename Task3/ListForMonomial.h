#pragma once

class ListForMonomialException
{
private:
	char* error;
	ListForMonomialException();


public:
	ListForMonomialException(const char*);
	ListForMonomialException(const ListForMonomialException&);
	~ListForMonomialException();

	const char* what();
};








class ListForMonomialItem
{
private:
	char name;
	int power;
	ListForMonomialItem* prev;
	ListForMonomialItem* next;
	
	ListForMonomialItem();
	ListForMonomialItem(const ListForMonomialItem&);
	ListForMonomialItem(ListForMonomialItem&&);
	
	

public:
	ListForMonomialItem& operator=(ListForMonomialItem&&);
	ListForMonomialItem& operator=(const ListForMonomialItem&);

	ListForMonomialItem(char, int);
	~ListForMonomialItem();

	ListForMonomialItem& operator*=(const ListForMonomialItem&);
	ListForMonomialItem& operator/=(const ListForMonomialItem&);

	ListForMonomialItem* GetPrev() const;
	ListForMonomialItem* GetNext() const;
	int GetPower() const;
	char GetName() const; 

	void SetPrev(ListForMonomialItem*);
	void SetNext(ListForMonomialItem*);
	void SetPower(int);
	void SetName(char);

	friend std::ostream& operator<<(std::ostream&, const ListForMonomialItem&);
	friend std::istream& operator>>(std::istream&, ListForMonomialItem&);
};












class ListForMonomial
{
protected:
	ListForMonomialItem* head;
	int size;

	ListForMonomialItem* First() const;
	ListForMonomialItem* Last() const;

	ListForMonomial();
	ListForMonomial(const ListForMonomial&);
	ListForMonomial(ListForMonomial&&);

	void Destroy();
	void Create();
	void Delete(ListForMonomialItem*);

public:
	
	~ListForMonomial();

	int Size() const;
	void Insert(char, int);
	void Delete(int);
	

	ListForMonomialItem& operator[](int) const;

	friend std::ostream& operator<<(std::ostream&, const ListForMonomial&);
};

