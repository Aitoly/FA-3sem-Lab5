#include<stdio.h>
#include<iostream>
#include <chrono>
#include <ctime>

#include"SqMatrix.h"

#define TEST_COUNT 10000



int mai()
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	//1	Determinant
	start = std::chrono::system_clock::now();
	for (int i = 0; i < TEST_COUNT; ++i)
	{
		SqMatrix A("A", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);
		Determinant(A);
	}
	end = std::chrono::system_clock::now();
	std::cout << "Determinant: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";

	//2	TransposeMatrix
	start = std::chrono::system_clock::now();
	for (int i = 0; i < TEST_COUNT; ++i)
	{
		SqMatrix A("A", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);
		TransposeMatrix(A);
	}
	end = std::chrono::system_clock::now();
	std::cout << "TransposeMatrix: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";

	//3	MultiplicativeInverse
	start = std::chrono::system_clock::now();
	for (int i = 0; i < TEST_COUNT; ++i)
	{
		SqMatrix A("A", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);
		MultiplicativeInverse(A);
	}
	end = std::chrono::system_clock::now();
	std::cout << "MultiplicativeInverse: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";

	//4	ExponentOfMatrix
	start = std::chrono::system_clock::now();
	for (int i = 0; i < TEST_COUNT; ++i)
	{
		SqMatrix A("A", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);
		ExponentOfMatrix(A);
	}
	end = std::chrono::system_clock::now();
	std::cout << "ExponentOfMatrix: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";

	//5	TraceOfMatrix
	start = std::chrono::system_clock::now();
	for (int i = 0; i < TEST_COUNT; ++i)
	{
		SqMatrix A("A", 3, 2., 2., 3., 4., 5., 6., 7., 8., 9.);
		TraceOfMatrix(A);
	}
	end = std::chrono::system_clock::now();
	std::cout << "ExponentOfMatrix: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";


	system("pause");
	return 0;
}