#include<iostream>
#include<stdarg.h>
#include<cstring>

#include "SqMatrix.h"

#define TESTa

////////////////////////////////////////////////////////
//		Исключения
////////////////////////////////////////////////////////

MatrixExeption::MatrixExeption() 
:error(NULL)
{}

MatrixExeption::MatrixExeption(const char* error_)
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

MatrixExeption::MatrixExeption(const MatrixExeption& Ex)
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

MatrixExeption::~MatrixExeption()
{
	if (error)
		delete[] error;
}

const char* MatrixExeption::what() const
{
	return error;
}

////////////////////////////////////////////////////////
//		КОНСТУКТОРЫ
////////////////////////////////////////////////////////
SqMatrix::SqMatrix()
{
	name = NULL;
	size = 0;
	data = NULL;
}

SqMatrix::SqMatrix(const double x)
{
	name = NULL;
	size = 1;
	data = new double*[1];
	*data = new double[1];
	**data = x;
}

SqMatrix::SqMatrix(const char* name_)
{
	if (name_)
	{
		name = new char[strlen(name_) + 1];
		strcpy(name, name_);
	}
	else
	{
		name = NULL;
	}

	data = NULL;
	size = 0;
}

SqMatrix::SqMatrix(const char* name_,const int size_,  ...)
{
	if (name_)
	{
		name = new char[strlen(name_) + 1];
		strcpy(name, name_);
	}
	else
	{
		name = NULL;
	}

	size = size_;

	data = new double*[size_];
	for (int i = 0; i < size_; ++i)
		*(data + i)= new double[size_];

	va_list args;
	va_start(args, size_);
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; ++j)
		{
			data[i][j] = va_arg(args, double);
		}
	}
	va_end(args);
}

SqMatrix::SqMatrix(const SqMatrix &Matrix)
{
	if (Matrix.name)
	{
		name = new char[strlen(Matrix.name) + 1];
		strcpy(name, Matrix.name);
	}
	else
	{
		name = NULL;
	}

	size = Matrix.size;

	if (Matrix.data)
	{
		data = new double*[size];
		for (int i = 0; i < size; ++i)
			*(data + i) = new double[size];

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; ++j)
			{
				data[i][j] = Matrix.data[i][j];
			}
		}
	}
	else
	{
		data = NULL;
	}
}

SqMatrix::SqMatrix(SqMatrix&& other)
{
	name = other.name;
	data = other.data;
	size = other.size;

	other.name = NULL;
	other.data = NULL;
	other.size = 0;
}

////////////////////////////////////////////////////////
//		ДЕСТРУКТОР
////////////////////////////////////////////////////////
SqMatrix::~SqMatrix()
{
	if (name)
	{
		delete[] name;
	}
	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			if (data[i])
			{
				delete[] data[i];
			}
		}
		delete[] data;
	}
}

////////////////////////////////////////////////////////
//		ОПЕРАТОРЫ
////////////////////////////////////////////////////////
SqMatrix& SqMatrix::operator=(const SqMatrix &Matrix)
{
	if (this == &Matrix)
	{
		return *this;
	}

	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			if (data[i])
			{
				delete[] data[i];
			}
		}
		delete[] data;
	}

	size = Matrix.size;

	if (Matrix.data)
	{
		data = new double*[size];
		for (int i = 0; i < size; ++i)
			*(data + i) = new double[size];

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; ++j)
			{
				data[i][j] = Matrix.data[i][j];
			}
		}
	}
	else
	{
		data = NULL;
	}

	return *this;
}

SqMatrix& SqMatrix::operator=(SqMatrix &&Matrix)
{
	if (this == &Matrix)
	{
		return *this;
	}

	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			if (data[i])
			{
				delete[] data[i];
			}
		}
		delete[] data;
	}

	name = Matrix.name;
	data = Matrix.data;
	size = Matrix.size;

	Matrix.name = NULL;
	Matrix.data = NULL;
	Matrix.size = 0;

	return *this;
}

SqMatrix& SqMatrix::operator+=(const SqMatrix &Matrix)
{
	if (size != Matrix.size)
	{
		throw MatrixExeption("Matrix dimension varies!");
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i][j] += Matrix.data[i][j];
		}
	}

	return *this;
}

SqMatrix& SqMatrix::operator-=(const SqMatrix &Matrix)
{
	if (size != Matrix.size)
	{
		throw MatrixExeption("Matrix dimension varies!");
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i][j] -= Matrix.data[i][j];
		}
	}

	return *this;
}

SqMatrix& SqMatrix::operator*=(const SqMatrix &Matrix)
{
	
	if (size != Matrix.size)
	{
		throw MatrixExeption("Matrix dimension varies!");
	}

	double** temp;
	temp = new double*[size];
	for (int i = 0; i < size; ++i)
		temp[i] = new double[size];

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			double t = 0;

			for (int k = 0; k < size; ++k)
			{
				t += data[i][k] * Matrix.data[k][j];
			}

			temp[i][j] = t;
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (data[i])
		{
			delete[] data[i];
		}
	}
	delete[] data;

	data = temp;
	
	return *this;
}

SqMatrix& SqMatrix::operator*=(const double x)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i][j] *= x;
		}
	}

	return *this;
}

SqMatrix& SqMatrix::operator/=(const double x)
{
	if (abs(x) < 0.000000001)
	{
		throw MatrixExeption("Division by zero!");
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i][j] /= x;
		}
	}

	return *this;
}


std::ostream& operator<<(std::ostream &os, const SqMatrix &Matrix)
{
	os << "Matrix ";

	if (Matrix.name)
	{
		os << Matrix.name << std::endl;
	}
	else
	{
		os << "<no name>" << std::endl;
	}

	if (Matrix.data)
	{
		for (int i = 0; i < Matrix.size; i++)
		{
			for (int j = 0; j < Matrix.size; ++j)
			{
				os << Matrix.data[i][j] << " ";
			}
			os << std::endl;
		}
	}
	else
	{
		os << "<no elements>" << std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream &is, SqMatrix &Matrix)
{
	// TODOL std::cin -> is
	char buff[BUFSIZ];
	//std::cout << "Enter the name of Matrix: ";
	is >> buff;

	int size;
	//std::cout << "Enter the size of Matrix: ";
	is >> size;

	Matrix.Rename(buff);
	double** data = Matrix.Resize(size);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			//std::cout << "Enter the [ "<< i <<"]["<< j <<"] element: ";
			is >> data[i][j];
		}
	}

	return is;
}


SqMatrix operator+(const SqMatrix &left, const SqMatrix &right)
{
	/*return SqMatrix(left) += right;*/
	SqMatrix temp(left);
	temp += right;
	return temp;
}

SqMatrix operator-(const SqMatrix &left, const SqMatrix &right)
{
	SqMatrix temp(left);
	temp -= right;
	return temp;
}

SqMatrix operator*(const SqMatrix &left, const SqMatrix &right)
{
	SqMatrix temp(left);
	temp *= right;
	return temp;
}

SqMatrix operator*(const SqMatrix &left, const double right)
{
	SqMatrix temp(left);
	temp *= right;
	return temp;
}

////////////////////////////////////////////////////////
//		ФУНКЦИИ
////////////////////////////////////////////////////////
void SqMatrix::Rename(const char* newName)
{
	if (name)
		delete[] name;

	name = new char[strlen(newName) + 1];

	strcpy(name, newName);
}

double** SqMatrix::Resize(const int newSize)
{
	if (size == newSize)
		return data;

	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			if (data[i])
			{
				delete[] data[i];
			}
		}
		delete[] data;
	}

	size = newSize;
	data = NULL;
	data = new double*[newSize];
	for (int i = 0; i < newSize; ++i)
		*(data + i) = new double[newSize];

	return data;
}

SqMatrix SqMatrix::Minor(int i_, int j_) const
{
	if (size < 2)
	{
		throw MatrixExeption("Error in CreateSmallerMatrix(...)! (Matrix.size < 2)");
	}
	if (j_ > size)
	{
		throw MatrixExeption("Error in CreateSmallerMatrix(...)! (j_ > Matrix.size)");
	}
	if (i_ > size)
	{
		throw MatrixExeption("Error in CreateSmallerMatrix(...)! (i_ > Matrix.size)");
	}

	SqMatrix tmp;
	tmp.size = size - 1;
	tmp.data = new double*[tmp.size];
	for (int i = 0; i < tmp.size; ++i)
		*(tmp.data + i) = new double[tmp.size];

	int current_i = 0;
	int current_j = 0;
	
	for (int i = 0; i < size; i++)
	{
		if (i != i_)
		{
			for (int j = 0; j < size; j++)
			{
				if (j != j_)
				{
					tmp.data[current_i][current_j] = data[i][j];
					current_j++;
				}
			}
			current_i++;
			current_j = 0;
		}
	}

	return tmp;
}

double SqMatrix::AlgebraicComplement(int i_, int j_) const
{
	SqMatrix t = Minor(i_, j_);

	if ((i_ + j_) % 2)
	{
		return -1 * Determinant(t);
	}
	else
	{
		return Determinant(t);
	}
}

void SqMatrix::Triangle()
{
	double det = Determinant(*this);
	if (abs(det) < 0.000000000001)
		throw MatrixExeption("Determinant = 0!");


	int i, j, k;
	double mu;
	
	for (k = 0; k < size; ++k)
	{
		for (i = k + 1; i < size; ++i)
		{
			mu = (double)data[i][k] / data[k][k];
			for (j = 0; j < size; ++j)
				data[i][j] -= data[k][j] * mu;
		}
	}

}

double* SqMatrix::Solve(int &resSize)//<- даблы
{
	double* result = NULL;

	if (abs(Determinant(*this)) < 0.000000000001)
	{
		resSize = 0;
		return NULL;
	}

	int n, m;
	//создаем массив
	n = size;
	m = size + 1;
	Bigger();
	//инициализируем после =

	for (int i = 0; i < n; ++i)
	{
		std::cout << "Line" << "[" << i + 1 << "]: ";

		std::cin >> data[i][size];
	}

	//Print();

	//Метод Гаусса
	//Прямой ход, приведение к верхнетреугольному виду
	float  tmp; 
	result = new double[m];

	for (int i = 0; i<n; i++)
	{
		tmp = data[i][i];
		for (int j = n; j >= i; j--)
			data[i][j] /= tmp;
		for (int j = i + 1; j<n; j++)
		{
			tmp = data[j][i];
			for (int k = n; k >= i; k--)
				data[j][k] -= tmp*data[i][k];
		}
	}

	//Print();
	//std::cout << std::endl;

	//обратный ход
	result[n - 1] = data[n - 1][n];
	for (int i = n - 2; i >= 0; i--)
	{
		result[i] = data[i][n];
		for (int j = i + 1; j<n; j++) 
			result[i] -= data[i][j] * result[j];
	}

	Smaller();
	resSize = size;
	return result;
}

void SqMatrix::Print()
{
	if (name)
	{
		std::cout << name << std::endl;
	}
	else
	{
		std::cout << "<no name>" << std::endl;
	}

	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size + 1; ++j)
			{
				std::cout << data[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	else
	{
		std::cout << "<no elements>" << std::endl;
	}
}

void SqMatrix::Bigger()
{
	double** tmp = data;
	data = NULL;

	data = new double*[size];
	for (int i = 0; i < size; ++i)
		*(data + i) = new double[size + 1];


	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size + 1; ++j)
		{
			if (j != size)
				data[i][j] = tmp[i][j];
			else
				data[i][j] = 0;
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (tmp[i])
		{
			delete[] tmp[i];
		}
	}
	delete[] tmp;
}

void SqMatrix::Smaller()
{
	double** tmp = data;
	data = NULL;

	data = new double*[size];
	for (int i = 0; i < size; ++i)
		*(data + i) = new double[size];


	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i][j] = tmp[i][j];
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (tmp[i])
		{
			delete[] tmp[i];
		}
	}
	delete[] tmp;
}


double Determinant(const SqMatrix &Matrix)
{
	if (!Matrix.data)
		throw MatrixExeption("Empty matirx!");

	if (Matrix.size == 1)
		return Matrix.data[0][0];
	if (Matrix.size == 2)
		return Matrix.data[0][0] * Matrix.data[1][1] - Matrix.data[1][0] * Matrix.data[0][1];

	double det = 0;

	for (int i = 0; i < Matrix.size; i++)
	{
		int sign;
		if (i % 2)
		{
			sign = -1;
		}
		else
		{
			sign = 1;
		}

		SqMatrix t = Matrix.Minor(0, i);

		det += sign * Matrix.data[0][i] * Determinant(t);
	}

	return det;
}

SqMatrix MultiplicativeInverse(const SqMatrix &Matrix)
{
	double det = Determinant(Matrix);

	if (abs(det) < 0.000000001)
	{
		throw MatrixExeption("det() = 0!");
	}

	SqMatrix tmp(Matrix.name);
	tmp.size = Matrix.size;
	tmp.data = new double*[tmp.size];
	for (int i = 0; i < tmp.size; ++i)
		*(tmp.data + i) = new double[tmp.size];

	for (int i = 0; i < tmp.size; ++i)
	{
		for (int j = 0; j < tmp.size; ++j)
		{
			tmp.data[i][j] = Matrix.AlgebraicComplement(i, j) / det;
		}
	}

	return TransposeMatrix(tmp);
}

SqMatrix TransposeMatrix(const SqMatrix &Matrix)
{
	SqMatrix tmp(Matrix.name);
	tmp.size = Matrix.size;
	tmp.data = new double*[tmp.size];
	for (int i = 0; i < tmp.size; ++i)
		*(tmp.data + i) = new double[tmp.size];

	for (int i = 0; i < tmp.size; ++i)
	{
		for (int j = 0; j < tmp.size; ++j)
		{
			tmp.data[i][j] = Matrix.data[j][i];
		}
	}

	return tmp;
}

double TraceOfMatrix(const SqMatrix &Matrix)
{
	double trace = 0;

	for (int i = 0; i < Matrix.size; ++i)
	{
		trace += Matrix.data[i][i];
	}

	return trace;
}

SqMatrix ExponentOfMatrix(const SqMatrix &Matrix)
{
	SqMatrix tmp(Matrix);

	for (int i = 0; i < tmp.size; ++i)
	for (int j = 0; j < tmp.size; ++j)
	{
		if (i == j)
			tmp.data[i][j] = 1;
		else
			tmp.data[i][j] = 0;
	}

	double eps = 0.00000000000000001;

	double n = 1;
	SqMatrix A(Matrix);

	for (double i = 1; 1 / n > eps; ++i)
	{
		n *= i;

		tmp += (A * (1 / n));

		A *= Matrix;
	}

	return tmp;
}