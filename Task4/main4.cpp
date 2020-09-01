#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>

#include"LargeInteger.h"

int main()
{
	try
	{
		
		LargeInteger l1("125", 1);
		std::cout << l1 << std::endl;
		
		LargeInteger l2 = l1.MulInt(51);
		std::cout << l2 << std::endl;

		std::cout << BinomialCoeff(100,50) << std::endl;//Метод вычисления биномиального коэффициента 

		std::cout << Fibonacci(10) << std::endl;//Вычисление nого элемента последовательности Фибоначчи
	}
	catch (LargeIntegerExсeption& Ex)
	{
		std::cout << Ex.what() << std::endl;
	}

	system("pause");
	return 0;
}