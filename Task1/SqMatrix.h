#pragma once
#pragma warning(disable:4996)
// exception
class MatrixExeption
{
private:
	char* error;
	MatrixExeption();


public:
	MatrixExeption(const char*);
	MatrixExeption(const MatrixExeption&);
	~MatrixExeption();
	
	const char* what() const;
};

#define TESTa

class SqMatrix
{
private:
	double** data;
	char* name;
	int size;

	SqMatrix& operator/=(const double);
	SqMatrix Minor(int, int) const;
	double AlgebraicComplement(int, int) const;
	
	void Print();
	void Triangle();
	void Bigger();
	void Smaller();

public:

	SqMatrix();
	explicit SqMatrix(const double);
	explicit SqMatrix(const char*);
	SqMatrix(const char*,const int,  ...);
	SqMatrix(const SqMatrix&);
	SqMatrix(SqMatrix&&);

	~SqMatrix();

	SqMatrix& operator=(const SqMatrix&);
	SqMatrix& operator=(SqMatrix&&);
	SqMatrix& operator+=(const SqMatrix&);
	SqMatrix& operator-=(const SqMatrix&);
	SqMatrix& operator*=(const SqMatrix&);
	SqMatrix& operator*=(const double);	

	void Rename(const char*);
	double** Resize(const int);
	
	double* Solve(int&);
	

	friend std::ostream& operator<<(std::ostream&, const SqMatrix&);
	friend std::istream& operator>>(std::istream&, SqMatrix&);


	friend double Determinant(const SqMatrix&);
	friend SqMatrix MultiplicativeInverse(const SqMatrix&);
	friend SqMatrix TransposeMatrix(const SqMatrix&);
	friend double TraceOfMatrix(const SqMatrix&);
	friend SqMatrix ExponentOfMatrix(const SqMatrix&);

};

SqMatrix operator+(const SqMatrix&, const SqMatrix&);
SqMatrix operator-(const SqMatrix&, const SqMatrix&);
SqMatrix operator*(const SqMatrix&, const SqMatrix&);
SqMatrix operator*(const SqMatrix&, const double);