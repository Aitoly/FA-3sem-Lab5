#include<stdio.h>
#include<iostream>

#include"SqMatrix.h"

int main()
{
	std::cout << "========== Constructors ==========" << std::endl;
	SqMatrix A1("A", 2, 1., 2., 3., 4.);
	SqMatrix B1;
	SqMatrix C1(5);
	SqMatrix D1("D");
	SqMatrix E1(A1);
	SqMatrix F1(B1);

	std::cout << A1 << std::endl;
	std::cout << B1 << std::endl;
	std::cout << C1 << std::endl;
	std::cout << D1 << std::endl;
	std::cout << E1 << std::endl;
	std::cout << F1 << std::endl;
	std::cout << "========== Operators type += ==========" << std::endl;
	SqMatrix A2("A", 2, 1., 2., 3., 4.);
	SqMatrix B2("B", 2, 1., 2., 3., 4.);
	SqMatrix C2("C", 3, 1., 2., 3., 4., 5., 6., 7., 8., 9.);

	try
	{
		A2 += C2;
	}
	catch (MatrixExeption& Ex)
	{
		std::cout << Ex.what() << std::endl;
	}
	std::cout << A2 << std::endl;

	A2 += B2;
	std::cout << A2 << std::endl;

	A2 *= 3;
	std::cout << A2 << std::endl;

	A2 *= B2;
	std::cout << A2 << std::endl;
	std::cout << "========== Operators type + ==========" << std::endl;
	SqMatrix A3("A", 2, 1., 2., 3., 4.);
	SqMatrix B3("B");
	SqMatrix C3("C");

	B3 = A3;
	std::cout << B3 << std::endl;

	C3 = B3 + A3;
	std::cout << C3 << std::endl;

	C3 = B3 - A3;
	std::cout << C3 << std::endl;

	C3 = B3 * A3;
	std::cout << C3 << std::endl;

	std::cout << "========== Functions ==========" << std::endl;
	SqMatrix A4("A4", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);
	SqMatrix B4("A4", 2, 2., 2., 3., 4.);

	std::cout << A4 << std::endl;

	std::cout << Determinant(A4) << std::endl;

	std::cout << TransposeMatrix(A4) << std::endl;

	std::cout << MultiplicativeInverse(A4) << std::endl;

	std::cout << TraceOfMatrix(A4) << std::endl;

	B4 = ExponentOfMatrix(A4);
	std::cout << B4 << std::endl;
	
	std::cout << "========== Solve ==========" << std::endl;
	SqMatrix A5("A5", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);

	std::cout << A5 << std::endl;

	int count;
	double* res = A5.Solve(count);

	for (int i = 0; i<count; i++)
		std::cout << res[i] << " ";
	std::cout << std::endl;

	if (res)
		delete[] res;

	system("pause");
	return 0;
}