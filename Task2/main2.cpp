#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

#include"Polynomial.h"
//3x^5+3x^4+3x^3+3x^2+3x+3
//2x^5+2x^4+2x^3+2x^2+2x+2

/*
0 Add - сложение
1 Sub - вычитание
2 Mult - умножение
3 Div Ц целочисленное деление
4 Mod Ц остаток от делени€
5 Eval Ц вычисление многочлена в заданной точке
6 Diff - дифференцирование
*/

int Get_Info(FILE *in, char* line)
{
	//без комментариев !!!
	if (!in)
	{
		return 0;
	}
	char c;

	while (!feof(in))
	{
		c = fgetc(in);
		c = tolower(c);

		
		if (isspace(c) || c == '\n' || c == EOF)
		{
			continue;
		}

		if (c == ';')
		{
			*line++ = c;
			*line = '\0';
			return 1;
		}
		else
		{
			*line++ = c;
		}
		
	}

	*line = '\0';
	return 0;
}

void Find_cmd_1st_2nd(char* line, char* cmd, char* first, char* second)
{
	*first = 0;
	*second = 0;
	*cmd = 0;
	char *ptr = line;

	int len = strlen(line);

	if (*(line + len - 1) == ';')
	{
		*(line + len - 1) = 0;
	}
	else
	{
		return;
	}

	if (*(line + len - 2) == ')')
	{
		*(line + len - 2) = 0;
	}
	else
	{
		return;
	}

	len -= 2;

	for (int i = 0; i < len; i++)
	{
		if (*ptr == '(')
		{
			*ptr = 0;
			strcpy(cmd, line);
			++ptr;
			strcpy(first, ptr);
			break;
		}
		++ptr;
	}

	len = strlen(first);
	ptr = first;

	for (int i = 0; i < len; i++)
	{
		if (*ptr == ',')
		{
			*ptr = 0;
			++ptr;
			strcpy(second, ptr);
			break;
		}
		++ptr;
	}
}


int main()
{
	Polynomial b;

	std::cin >> b;

	std::cout << b << std::endl;



	FILE *f;
	f = fopen("in.txt", "r");
	if (!f)
	{
		std::cout << "I can`t open file in.txt!\n";
		system("pause");
		exit(1);
	}

	char line[BUFSIZ], cmd[BUFSIZ], first[BUFSIZ], second[BUFSIZ];
	Polynomial tmp;
	double tmp_double;

	while (!feof(f))
	{
		Get_Info(f, line);

		if (strlen(line) > 0)
		{

			//std::cout << line << std::endl;

			Find_cmd_1st_2nd(line, cmd, first, second);

			std::cout << std::endl << "cmd: " << cmd << std::endl;
			std::cout << "first: " << first << std::endl;
			std::cout << "second: " << second << std::endl;

			int lenOfFirst = strlen(first);
			int lenOfSecond = strlen(second);

			try{

				if (!strcmp("add", cmd))
				{
					if (lenOfSecond)
					{
						Polynomial left(first);
						Polynomial right(second);

						tmp = left + right;
						std::cout << "RESULT = " << tmp << std::endl;
					}
					else
					{
						Polynomial left(first);

						tmp = tmp + left;
						std::cout << "RESULT = " << tmp << std::endl;
					}
				}
				else if (!strcmp("sub", cmd))
				{
					if (lenOfSecond)
					{
						Polynomial left(first);
						Polynomial right(second);

						tmp = left - right;
						std::cout << "RESULT = " << tmp << std::endl;
					}
					else
					{
						Polynomial left(first);

						tmp = tmp - left;
						std::cout << "RESULT = " << tmp << std::endl;
					}
				}
				else if (!strcmp("mult", cmd))
				{
					if (lenOfSecond)
					{
						Polynomial left(first);
						Polynomial right(second);

						tmp = left * right;
						std::cout << "RESULT = " << tmp << std::endl;
					}
					else
					{
						Polynomial left(first);

						tmp = tmp * left;
						std::cout << "RESULT = " << tmp << std::endl;
					}
				}
				else if (!strcmp("div", cmd))
				{
					if (lenOfSecond)
					{
						Polynomial left(first);
						Polynomial right(second);

						tmp = left / right;
						std::cout << "RESULT = " << tmp << std::endl;
					}
					else
					{
						Polynomial left(first);

						tmp = tmp / left;
						std::cout << "RESULT = " << tmp << std::endl;
					}
				}
				else if (!strcmp("mod", cmd))
				{
					if (lenOfSecond)
					{
						Polynomial left(first);
						Polynomial right(second);

						tmp = left % right;
						std::cout << "RESULT = " << tmp << std::endl;
					}
					else
					{
						Polynomial left(first);

						tmp = tmp % left;
						std::cout << "RESULT = " << tmp << std::endl;
					}
				}
				else if (!strcmp("eval", cmd))
				{
					if (lenOfSecond)
					{
						Polynomial left(first);
						int x = atoi(second);

						tmp_double = left.Solve(x);
						std::cout << "RESULT = " << tmp_double << std::endl;
					}
					else
					{
						int x = atoi(first);

						tmp_double = tmp.Solve(x);
						std::cout << "RESULT = " << tmp_double << std::endl;
					}
				}
				else if (!strcmp("diff", cmd))
				{
					if (lenOfFirst)
					{
						Polynomial left(first);

						tmp = left.Derivative();
						std::cout << "RESULT = " << tmp << std::endl;
					}
					else
					{
						tmp = tmp.Derivative();
						std::cout << "RESULT = " << tmp << std::endl;
					}
				}
				else
				{
					std::cout << "Bad command! :: " << line << std::endl;
				}
			}
			catch (PolynomialExeption &Ex)
			{
				std::cout << Ex.what() << std::endl;
			}
			catch (ListExeption &Ex)
			{
				std::cout << Ex.what() << std::endl;
			}
		}

	}

	system("pause");
	return 0;
}