#pragma warning(disable:4996)
#include<iostream>

#include"Polynomial.h"





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
	

	

	if (*(line + strlen(line) - 1) == ';')
	{
		*(line + strlen(line) - 1) = 0;
	}
	else
	{
		return;
	}

	if (*(line + strlen(line) - 1) == '>')
	{
		*(line + strlen(line) - 1) = 0;
	}
	else
	{
		return;
	}

	if (*(line) == '<')
	{
		++line;
	}
	else
	{
		return;
	}





	*first = 0;
	*second = 0;
	*cmd = 0;
	char *ptr = line;
	int len = strlen(line);




	for (int i = 0; i < len; i++)
	{
		if (*ptr == '>')
		{
			*ptr = 0;
			strcpy(first, line);
			++ptr;
			strcpy(cmd, ptr);
			break;
		}
		++ptr;
	}

	len = strlen(cmd);
	ptr = cmd;

	for (int i = 0; i < len; i++)
	{
		if (*ptr == '<')
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
	/*Polynomial b;

	std::cin >> b;

	std::cout << b << std::endl;*/

	/*Polynomial p1("3x^2+4");
	Polynomial p2("5y^2+6");

	std::cout << (p1+p2) << std::endl;
	p1 += p2;
	std::cout << p1 << std::endl;

	system("pause");
	return 0;*/

	FILE *f;
	f = fopen("in.txt", "r");
	if (!f)
	{
		std::cout << "I can`t open file in.txt!\n";
		system("pause");
		exit(1);
	}

	char line[BUFSIZ], cmd[BUFSIZ], first[BUFSIZ], second[BUFSIZ];
	
	while (!feof(f))
	{
		Get_Info(f, line);

		if (strlen(line) > 0)
		{

			//std::cout << line << std::endl;

			Find_cmd_1st_2nd(line, cmd, first, second);

			//std::cout << std::endl << "cmd: " << cmd << std::endl;
			//std::cout << "first: " << first << std::endl;
			//std::cout << "second: " << second << std::endl;

			try{

				if (!strcmp("+", cmd))
				{
					Polynomial left(first);
					Polynomial right(second);
					std::cout << left + right << std::endl;
				}
				else if (!strcmp("-", cmd))
				{
					Polynomial left(first);
					Polynomial right(second);
					std::cout << left - right << std::endl;
				}
				else if (!strcmp("*", cmd))
				{
					Polynomial left(first);
					Polynomial right(second);
					std::cout << left * right << std::endl;
				}
				else if (!strcmp("==", cmd))
				{
					Polynomial left(first);
					Polynomial right(second);
					std::cout << (left == right) << std::endl;
				}
				else if (!strcmp("!=", cmd))
				{
					Polynomial left(first);
					Polynomial right(second);
					std::cout << (left != right) << std::endl;
				}
				else
				{
					std::cout << "Bad command! :: " << cmd << std::endl;
				}
			}
			catch (ListForMonomialException &Ex)
			{
				std::cout << Ex.what() << std::endl;
			}
			catch (PolynomialExeption &Ex)
			{
				std::cout << Ex.what() << std::endl;
			}
			catch (ListForPolynomialException &Ex)
			{
				std::cout << Ex.what() << std::endl;
			}
		}

	}

	system("pause");
	return 0;
}