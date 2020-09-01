#pragma warning(disable:4996)
#include<iostream>
#include<cstring>
#include<algorithm>

#include "LargeInteger.h"

#define DEBUG_MULTc


////////////////////////////////////////////////////////
//		Èñêëş÷åíèÿ
////////////////////////////////////////////////////////

LargeIntegerExñeption::LargeIntegerExñeption()
:error(NULL)
{}

LargeIntegerExñeption::LargeIntegerExñeption(const char* error_)
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

LargeIntegerExñeption::LargeIntegerExñeption(const LargeIntegerExñeption& Ex)
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

LargeIntegerExñeption::~LargeIntegerExñeption()
{
	if (error)
		delete[] error;
}

const char* LargeIntegerExñeption::what()
{
	return error;
}

////////////////////////////////////////////////////////
//		ÊÎÍÑÒÓÊÒÎĞÛ
////////////////////////////////////////////////////////
LargeInteger::LargeInteger()
{
	data = NULL;
	size = 0;
	base = 0;
	plus = true;
}

LargeInteger::LargeInteger(const char* num, int base_)
{
	if (base_ == 0)
	{
		throw LargeIntegerExñeption("base_ = 0");
	}

	int len;

	char *in = NULL;
	char* ptr;
	
	if (isdigit(*num))
	{
		len = strlen(num);
		plus = true;
		in = new char[len + 1];
		strcpy(in, num);
		ptr = in + len;
	}
	else
	{
		len = strlen(num + 1);
		if (*num == '+')
		{
			plus = true;
		}
		else if (*num == '-')
		{
			plus = false;
		}
		else
		{
			throw LargeIntegerExñeption("Invalid sign");
		}
		in = new char[len + 1];
		strcpy(in, num+1);
		ptr = in + len;
	}

	
	int current = 0;

	base = base_;
	
	size = (int)((double)len / base + 0.5);

	data = new int[size];

	for (int i = 0; i < size; ++i)
	{
		ptr -= base;
		if (ptr < in)
		{
			ptr = in;
		}
		current = atoi(ptr);
		*ptr = 0;
		data[i] = current;
	}

	delete[] in;
}

LargeInteger::LargeInteger(int num)
{
	int tmp = num;
	int count = 0;

	while (tmp)
	{
		tmp /= 10;
		++count;
	}

	if (!count)
	{
		data = new int[1];
		data[0] = 0;
		size = 1;
		base = 1;
		plus = true;
	}
	else
	{
		data = new int[count];
		size = count;
		base = 1;
		if (num < 0)
		{
			plus = false;
		}
		else
		{
			plus = true;
		}

		for (int i = 0; num; ++i)
		{
			data[i] = num % 10;
			num /= 10;
		}
	}
}

LargeInteger::LargeInteger(const LargeInteger &large)
{
	if (large.size > 0)
	{
		data = new int[large.size];
		size = large.size;
		base = large.base;
		plus = large.plus;

		for (int i = 0; i < size; ++i)
		{
			*(data + i) = *(large.data + i);
		}
	}
	else
	{
		data = NULL;
		size = 0;
		base = 0;
		plus = true;
	}
}

LargeInteger::LargeInteger(const LargeInteger &large, bool plus_)
{
	if (large.size > 0)
	{
		data = new int[large.size];
		size = large.size;
		base = large.base;
		plus = plus_;

		for (int i = 0; i < size; ++i)
		{
			*(data + i) = *(large.data + i);
		}
	}
	else
	{
		data = NULL;
		size = 0;
		base = 0;
		plus = plus_;
	}
}

LargeInteger::LargeInteger(const LargeInteger &large, int begin, int end)
{
	if (begin < 0)
		begin = 0;

	if (begin > large.size - 1)
		begin = large.size - 1;

	if (end < 0)
		end = 0;

	if (end > large.size)
		end = large.size;

	int n = end - begin;

	data = new int[n];
	size = n;
	base = large.base;
	plus = true;

	n = 0;

	for (int i = begin; i < end; ++i, ++n)
	{
		data[n] = large.data[i];
	}
}

LargeInteger::LargeInteger(LargeInteger &&large)
{
	data = large.data;
	large.data = NULL;

	size = large.size;
	large.size = 0;

	base = large.base;
	large.base = 0;

	plus = large.plus;
}



////////////////////////////////////////////////////////
//		ÄÅÑÒĞÓÊÒÎĞ
////////////////////////////////////////////////////////
LargeInteger::~LargeInteger()
{
	if (size > 0)
		delete[] data;
}

////////////////////////////////////////////////////////
//		ÎÏÅĞÀÒÎĞÛ
////////////////////////////////////////////////////////
LargeInteger& LargeInteger::operator=(const LargeInteger &large)
{
	if (&large == this)
	{
		return *this;
	}

	if (size > 0)
		delete[] data;

	size = large.size;
	base = large.base;
	plus = large.plus;

	data = new int[size];

	for (int i = 0; i < size; ++i)
	{
		*(data + i) = *(large.data + i);
	}

	return *this;
}

LargeInteger& LargeInteger::operator=(LargeInteger &&large)
{
	if (&large == this)
	{
		return *this;
	}

	if (size > 0)
		delete[] data;

	data = large.data;
	large.data = NULL;

	size = large.size;
	large.size = 0;

	base = large.base;
	large.base = 0;

	plus = large.plus;

	return *this;
}


LargeInteger& LargeInteger::operator+=(const LargeInteger &large)
{
	
	if (large.size == 0)
		throw LargeIntegerExñeption("Empty LargeInteger in operator+=");

	if (large.base != base)
		throw LargeIntegerExñeption("Base error in operator+=");

	LargeInteger right(large, true);

	if (plus && !large.plus)//åñëè 12 + (-6), òî 12 - 6
	{
		return operator-=(right);
	}

	if (!plus && large.plus)//åñëè -12 + 6, òî 6 - 12
	{
		plus = true;
		Swap(right);
		return operator-=(right);
	}

//ñêàëàäûâàåò òîëüêî 2 ïîëîæèòåëüíûõ ÷èñëà(çàíàêè ìèìî)
	if (size <= right.size)
		SizeToUp(right.size + 1);

	int n = std::min(size, right.size);
	
	int carry = 0, tmp;
	int base_ = (int)pow(10, base);

	int i;
	for (i = 0; i < n; ++i)
	{
		tmp = data[i] + right.data[i] + carry;
		carry = tmp / base_;
		data[i] = tmp % base_;
	}

	if (carry)
	{
		data[i] += carry;
	}

	Normal();

	return *this;
}

LargeInteger& LargeInteger::operator-=(const LargeInteger &large)
{
	if (large.size == 0)
		throw LargeIntegerExñeption("Empty LargeInteger in operator-=");

	if (large.base != base)
		throw LargeIntegerExñeption("Base error in operator-=");

	{
		LargeInteger right(large);
		if (operator==(right))
		{
			(*this) = LargeInteger("0", base);
			return *this;
		}
	}

	LargeInteger right(large, true);

	if (plus && !large.plus)//åñëè 12 - (-6), òî 12 + 6
	{
		return operator+=(right);
	}

	if (!plus && large.plus)//åñëè -12 - 6, òî -(12 + 6)
	{
		plus = true; 
		operator+=(right);
		plus = false;
		return *this;
	}

	if (!plus && !large.plus)//åñëè -12 - (-6), òî -12 + 6
	{
		plus = true;
		if (operator<(right))
		{
			Swap(right);
		}
		else
		{
			plus = false;
		}
	}

	if (plus && large.plus && operator<(right))//åñëè 6 - 12, òî -(12 - 6)
	{
		Swap(right);
		plus = false;
	}


//âû÷èòàåò è áîëüøåãî ìàíüøåå(çàíàêè ìèìî)
	if (size <= right.size)
		SizeToUp(right.size + 1);

	int n = std::min(size, right.size);

	int carry = 0, tmp;
	int base_ = (int)pow(10, base);

	int i;
	for (i = 0; i < n; ++i)
	{
		tmp = data[i] - right.data[i] + carry;
		if (tmp >= 0)
		{
			data[i] = tmp;
			carry = 0;
		}
		else
		{
			tmp += base_;
			carry = -1;
			data[i] = tmp;
		}
	}

	if (carry)
	{
		tmp = data[i] + carry;
		if (tmp >= 0)
		{
			data[i] = tmp;
			carry = 0;
		}
		else
		{
			tmp += base_;
			data[i] = tmp;
		}
	}

	Normal();

	return *this;
}

/*LargeInteger& LargeInteger::operator*=(const LargeInteger &large)
{
	bool this_plus = plus; 
	plus = true;
	bool other_plus = large.plus;

	(*this) = KaratsubaMul(large);

	if ((this_plus && !other_plus) || (!this_plus && other_plus))
	{
		plus = false;
	}

	return *this;
}*/

LargeInteger& LargeInteger::operator*=(const LargeInteger &large)
{
	bool this_plus = plus;
	plus = true;
	bool other_plus = large.plus;

	
	if (size == 0 || large.size == 0)
	{
		(*this) = LargeInteger("0", base);
		return *this;
	}
		

	LargeInteger tmp = LargeInteger("0", base);

	tmp.data = new int[size*3];

	for (int i = 0; i < size; ++i)
	{
		tmp.data[i] = 0;
	}

	for (int i = 0; i < large.size; ++i)
	{
		LargeInteger tmp_ = (*this).MulInt(large.data[i]);
		//std::cout << "1    " << tmp_ << std::endl;
		
		tmp_.SizeToEnd(tmp_.size + i);
		
		//std::cout << "2    " << tmp_ << std::endl;
		tmp += tmp_;
		//std::cout << "tmp    " << tmp << std::endl;
	}

	(*this) = tmp;

	return *this;


	if ((this_plus && !other_plus) || (!this_plus && other_plus))
	{
		plus = false;
	}

	return *this;
}

LargeInteger& LargeInteger::operator*=(int x)
{
	SizeToUp(size + 1);

	int base_ = (int)pow(10, base);

	int i, carry = 0, tmp;
	for (i = 0; i < size; ++i)
	{
		tmp = data[i] * x + carry;
		if (tmp >= base)
		{
			carry = tmp / base_;
		}
		else
		{
			carry = 0;
		}
		data[i] = tmp % base_;
	}

	if (carry)
	{
		data[i] += carry;
	}

	Normal();
	return *this;
}

LargeInteger LargeInteger::MulInt(int x)
{
	LargeInteger res(*this);

	res.SizeToUp(size + 3);

	int base_ = (int)pow(10, res.base);

	int i, carry = 0, tmp;
	for (i = 0; i < res.size; ++i)
	{
		tmp = res.data[i] * x + carry;
		if (tmp >= base_)
		{
			carry = tmp / base_;
		}
		else
		{
			carry = 0;
		}
		res.data[i] = tmp % base_;
	}

	if (carry)
	{
		res.data[i] += carry;
	}

	res.Normal();
	return res;
}

LargeInteger& LargeInteger::operator/=(const LargeInteger &large)
{
	if (large.size == 0)
	{
		throw LargeIntegerExñeption("Empty Polynomial in operator/=");
	}

	if (large.base != base)
		throw LargeIntegerExñeption("Base error in operator/=");

	bool this_plus = plus;
	plus = true;
	bool other_plus = large.plus;

	LargeInteger tmp(large, true);

	Normal();
	tmp.Normal();

	if (size < tmp.size)
	{
		(*this) = LargeInteger("0", base);

		return *this;
	}

	if (tmp.size == 0 && tmp.data[0] == 0)
	{
		throw LargeIntegerExñeption("Division by zero");
	}

	LargeInteger res;
	int zeros = size - tmp.size;
	res.base = base;


	while (operator>(tmp))//size >= tmp.size
	{
		LargeInteger minus(tmp);
		minus.SizeToEnd(minus.size + zeros);

		//std::cout << "minus: " << minus << std::endl;

		int m = 0;
		while (operator>=(minus))
		{
			operator-=(minus);
			++m;
			//std::cout << "me: " << (*this) << std::endl;
		}

		res.SizeToEnd(res.size + 1);
		res.data[0] = m;
		--zeros;
	}

	if (zeros != -1)
	{
		res.SizeToEnd(res.size + zeros + 1);
	}

	res.Normal();

	(*this) = res;

	if ((this_plus && !other_plus) || (!this_plus && other_plus))
	{
		plus = false;
	}

	return *this;
}

LargeInteger& LargeInteger::operator%=(const LargeInteger &large)
{
	if (large.size == 0)
	{
		throw LargeIntegerExñeption("Empty Polynomial in operator%=");
	}

	if (large.base != base)
		throw LargeIntegerExñeption("Base error in operator%=");

	LargeInteger tmp(large);

	Normal();
	tmp.Normal();

	if (size < tmp.size)
	{
		(*this) = LargeInteger("0", base);

		return *this;
	}

	if (tmp.size == 0 && tmp.data[0] == 0)
	{
		throw LargeIntegerExñeption("Division by zero");
	}

	int zeros = size - tmp.size;

	while (operator>(tmp))
	{
		LargeInteger minus(tmp);
		minus.SizeToEnd(minus.size + zeros);

		//std::cout << "minus: " << minus << std::endl;

		while (operator>(minus))
		{
			operator-=(minus);
		}
		--zeros;
	}

	return *this;
}

bool LargeInteger::operator<(LargeInteger &large)
{
	if (size == 0 || large.size == 0)
	{
		throw LargeIntegerExñeption("Empty LargeInteger in operator<=");
	}

	Normal();
	large.Normal();

	if (size == 1 && large.size == 1 && data[0] == 0 && large.data[0] == 0)
	{
		return false;
	}

	if (plus)
	{
		if (!large.plus)
			return false;

		if (size > large.size)
			return false;

		if (size < large.size)
			return true;

		for (int i = large.size - 1; i >= 0; --i)
		{
			if (data[i] < large.data[i])
				return true;
			if (data[i] > large.data[i])
				return false;
		}
		return false;
	}

	if (!plus)
	{
		if (large.plus)
			return true;

		if (size > large.size)
			return true;

		if (size < large.size)
			return false;

		for (int i = large.size - 1; i >= 0; --i)
		{
			if (data[i] > large.data[i])
				return true;
			if (data[i] < large.data[i])
				return false;
		}
		return false;
	}
}

bool LargeInteger::operator==(LargeInteger &large)
{
	if (size == 0 || large.size == 0)
	{
		throw LargeIntegerExñeption("Empty LargeInteger in operator==");
	}

	Normal();
	large.Normal();

	if (size != large.size)
	{
		return false;
	}

	if (plus != large.plus)
	{
		return false;
	}

	for (int i = 0; i < size; ++i)
	{
		if (data[i] != large.data[i])
			return false;
	}

	return true;
}

bool LargeInteger::operator>(LargeInteger &large)
{
	return !operator<(large) && !operator==(large);
}

bool LargeInteger::operator>=(LargeInteger &large)
{
	return operator>(large) || operator==(large);
}

////////////////////////////////////////////////////////
//		ÎÏÅĞÀÒÎĞÛ íå êîìïîíåíòûå
////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &os, const LargeInteger &large)
{
	//os << "base: " << large.base << "int:  ";

	if (large.size && !large.plus)
	{
		os << "- ";
	}

	for (int i = large.size - 1; i >= 0; --i)
	{
		os << *(large.data + i) << " ";
	}

	return os;
}

std::istream& operator>>(std::istream &is, LargeInteger &large)
{
	char buff[BUFSIZ];
	int base;

	is >> base;

	is >> buff;

	large = LargeInteger(buff, base);

	return is;
}





LargeInteger operator+(const LargeInteger &left, const LargeInteger &right)
{
	LargeInteger tmp(left);
	tmp += right;
	return tmp;
}

LargeInteger operator-(const LargeInteger &left, const LargeInteger &right)
{
	LargeInteger tmp(left);
	tmp -= right;
	return tmp;
}

LargeInteger operator*(const LargeInteger &left, const LargeInteger &right)
{
	LargeInteger tmp(left);
	tmp *= right;
	return tmp;
}

LargeInteger operator/(const LargeInteger &left, const LargeInteger &right)
{
	LargeInteger tmp(left);
	tmp /= right;
	return tmp;
}

LargeInteger operator%(const LargeInteger &left, const LargeInteger &right)
{
	LargeInteger tmp(left);
	tmp %= right;
	return tmp;
}

////////////////////////////////////////////////////////
//		ÔÓÍÊÖÈÈ
////////////////////////////////////////////////////////
void LargeInteger::SizeToUp(int n)
{
	int* dataCopy = data;
	data = new int[n];

	for (int i = 0; i < n; ++i)
	{
		if (i < size)
			data[i] = dataCopy[i];
		else
			data[i] = 0;
	}
	size = n;
}

void LargeInteger::SizeToEnd(int n)
{
	int countZero = n - size;

	if (countZero > 0)
	{
		int* dataCopy = data;
		data = new int[n];


		int i;
		for (i = 0; i < countZero; ++i)
		{
			data[i] = 0;
		}

		for (; i < n; ++i)
		{
			data[i] = dataCopy[i - countZero];
		}

		size = n;
	}
}

void LargeInteger::SizeTo2()
{
	if (size == 2 && data[1] == 0)
	{
		size = 1;
		int dataCopy = data[0];
		delete[] data;
		data = new int[size];
		data[0] = dataCopy;
				
		return;
	}

	if (size & 1)
	{
		if (data[size - 1] == 0)
		{
			int* dataCopy = data;
			data = new int[size--];

			for (int i = 0; i < size; ++i)
			{
				data[i] = dataCopy[i];
			}
			delete[] dataCopy;
			return;
		}


		int* dataCopy = data;
		data = new int[size + 1];

		for (int i = 0; i < size + 1; ++i)
		{
			if (i < size)
				data[i] = dataCopy[i];
			else
				data[i] = 0;
		}
		++size;
		delete[] dataCopy;
	}
}


void LargeInteger::Normal()
{
	if (size > 1)
	{
		int n = size;
		for (int i = size - 1; i > 0; --i)
		{
			if (data[i] != 0)
				break;
			else
				size--;
		}

		SizeToUp(size);
	}
}

void LargeInteger::Swap(LargeInteger &large)
{
	std::swap(data, large.data);
	std::swap(size, large.size);
	std::swap(base, large.base);
	std::swap(plus, large.plus);
}

/*LargeInteger LargeInteger::KaratsubaMul(const LargeInteger &other) const
{
	if (other.size == 0)
		throw LargeIntegerExñeption("Empty LargeInteger in KaratsubaMul");

	if (other.base != base)
		throw LargeIntegerExñeption("Base error in KaratsubaMul");

	

	if (size == 1 && other.size == 1)
	{
		return Mul(other);
	}

	LargeInteger res;

	LargeInteger X(*this, true);
	LargeInteger Y(other, true);

#ifdef DEBUG_MULT
	std::cout << std::endl << "X: " << X << std::endl;
	std::cout << "Y: " << Y << std::endl;
#endif

	X.Normal();
	Y.Normal();

	X.SizeTo2();
	Y.SizeTo2();

	if (X.size == 1 && Y.size == 1)
	{
		return X.Mul(Y);
	}

#ifdef DEBUG_MULT
	std::cout << std::endl << "X: " << X << std::endl;
	std::cout << "Y: " << Y << std::endl;
#endif

	const int n = std::max(X.size, Y.size);

	X.SizeToUp(n);
	Y.SizeToUp(n);

	int k = n / 2;

	LargeInteger Xr(X, 0, k);
	LargeInteger Xl(X, k, INT_MAX);
	LargeInteger Yr(Y, 0, k);
	LargeInteger Yl(Y, k, INT_MAX);

#ifdef DEBUG_MULT
	std::cout << std::endl << "Xl: " << Xl;
	std::cout << "   Xr: " << Xr << std::endl;
	std::cout << "Yl: " << Yl;
	std::cout <<    "Yr: " << Yr << std::endl;
#endif

	LargeInteger P1 = Xl.KaratsubaMul(Yl);
	LargeInteger P2 = Xr.KaratsubaMul(Yr);
	LargeInteger P3 = (Xr + Xl).KaratsubaMul(Yl + Yr);
	P1.Normal();
	P2.Normal();
	P3.Normal();

#ifdef DEBUG_MULT
	std::cout << std::endl << "P1: " << P1 << std::endl;
	std::cout << "P2: " << P2 << std::endl;
	std::cout << "P3: " << P3 << std::endl;
#endif

	P3 -= P2;
	P3 -= P1;

	P1.SizeToEnd(P1.size + n);
	P3.SizeToEnd(P3.size + k);

#ifdef DEBUG_MULT
	std::cout << std::endl << "P1: " << P1 << std::endl;
	std::cout << "P2: " << P2 << std::endl;
	std::cout << "P3: " << P3 << std::endl;
#endif

	res = P1 + P2 + P3;

#ifdef DEBUG_MULT
	std::cout << std::endl << "res: " << res << std::endl;
#endif

	return res;
}*/

/*LargeInteger LargeInteger::KaratsubaMul(const LargeInteger &other) const
{
	if (other.size == 0)
		throw LargeIntegerExñeption("Empty LargeInteger in KaratsubaMul");

	if (other.base != base)
		throw LargeIntegerExñeption("Base error in KaratsubaMul");

	int index = 1;

	LargeInteger* X = new LargeInteger(*this, true);
	LargeInteger* Y = new LargeInteger(other, true);

	LargeInteger* Res = new LargeInteger();

	LargeInteger* Xr = NULL;
	LargeInteger* Xl = NULL;
	LargeInteger* Yr = NULL;
	LargeInteger* Yl = NULL;

	LargeInteger* Xr_tmp = NULL;
	LargeInteger* Xl_tmp = NULL;
	LargeInteger* Yr_tmp = NULL;
	LargeInteger* Yl_tmp = NULL;

	LargeInteger* P1 = NULL;
	LargeInteger* P2 = NULL;
	LargeInteger* P3 = NULL;

	LargeInteger* P1_tmp = NULL;
	LargeInteger* P2_tmp = NULL;
	LargeInteger* P3_tmp = NULL;

	while (index)
	{
		if (X[index].size == 1 && Y[index].size == 1)
		{
			Res[index] = X[index].Mul(Y[index]);
			--index;
			continue;
		}


#ifdef DEBUG_MULT
		std::cout << std::endl << "X: " << X << std::endl;
		std::cout << "Y: " << Y << std::endl;
#endif

		X[index].Normal();
		Y[index].Normal();

		X[index].SizeTo2();
		Y[index].SizeTo2();

		if (X[index].size == 1 && Y[index].size == 1)
		{
			Res[index] = X[index].Mul(Y[index]);
			--index;
			continue;
		}

#ifdef DEBUG_MULT
		std::cout << std::endl << "X: " << X << std::endl;
		std::cout << "Y: " << Y << std::endl;
#endif

		const int n = std::max(X[index].size, Y[index].size);

		X[index].SizeToUp(n);
		Y[index].SizeToUp(n);

		int k = n / 2;

		Xr_tmp = Xr;
		Xl_tmp = Xl;
		Yr_tmp = Yr;
		Yl_tmp = Yl;

		Xr = new LargeInteger[index];
		Xl = new LargeInteger[index];
		Yr = new LargeInteger[index];
		Yl = new LargeInteger[index];

		for (int i = 0; i < index - 1; ++i)
		{
			Xr[index] = Xr_tmp[index];
			Xl[index] = Xl_tmp[index];
			Yr[index] = Yr_tmp[index];
			Yl[index] = Yl_tmp[index];
		}

		LargeInteger* to;

		to = Xr + index;
		to = new LargeInteger(X[index], 0, k);

		to = Xl + index;
		to = new LargeInteger(X[index], k, INT_MAX);

		to = Yr + index;
		to = new LargeInteger(Y[index], 0, k);

		to = Yl + index;
		to = new LargeInteger(Y[index], k, INT_MAX);


#ifdef DEBUG_MULT
		std::cout << std::endl << "Xl: " << Xl;
		std::cout << "   Xr: " << Xr << std::endl;
		std::cout << "Yl: " << Yl;
		std::cout << "Yr: " << Yr << std::endl;
#endif
		P1_tmp = P1;
		P2_tmp = P2;
		P3_tmp = P3;

		P1 = new LargeInteger[index];
		P2 = new LargeInteger[index];
		P3 = new LargeInteger[index];

		for (int i = 0; i < index - 1; ++i)
		{
			P1[index] = P1_tmp[index];
			P2[index] = P2_tmp[index];
			P3[index] = P3_tmp[index];
		}


		LargeInteger P1 = Xl.KaratsubaMul(Yl);
		LargeInteger P2 = Xr.KaratsubaMul(Yr);
		LargeInteger P3 = (Xr + Xl).KaratsubaMul(Yl + Yr);
		P1.Normal();
		P2.Normal();
		P3.Normal();

#ifdef DEBUG_MULT
		std::cout << std::endl << "P1: " << P1 << std::endl;
		std::cout << "P2: " << P2 << std::endl;
		std::cout << "P3: " << P3 << std::endl;
#endif

		P3 -= P2;
		P3 -= P1;

		P1.SizeToEnd(P1.size + n);
		P3.SizeToEnd(P3.size + k);

#ifdef DEBUG_MULT
		std::cout << std::endl << "P1: " << P1 << std::endl;
		std::cout << "P2: " << P2 << std::endl;
		std::cout << "P3: " << P3 << std::endl;
#endif

		res = P1 + P2 + P3;

#ifdef DEBUG_MULT
		std::cout << std::endl << "res: " << res << std::endl;
#endif

	
	}
	return res;
}*/

LargeInteger LargeInteger::Mul(const LargeInteger &other) const
{
	if (other.size != 1 || size != 1)
		throw LargeIntegerExñeption("Size error in Mul");

	if (other.base != base)
		throw LargeIntegerExñeption("Base error in Mul");


	LargeInteger res;

	res.size = 2;
	res.data = new int[res.size];
	res.base = base;
	res.plus = true;

	res.data[0] = 0;
	res.data[1] = 0;

	int base_ = (int)pow(10, base);

	int tmp = data[0] * other.data[0];

	if (tmp > base)
	{
		res.data[1] = tmp / base_;
	}

	res.data[0] = tmp % base_;

	res.Normal();

	return res;
}

////////////////////////////////////////////////////////
//		ÔÓÍÊÖÈÈ ïî çàäàíèş
////////////////////////////////////////////////////////
LargeInteger LargeInteger::Pow(int b)
{
	if (b < 0)
	{
		throw LargeIntegerExñeption("Power < 0 in Pow");
	}

	if (size == 0)
	{
		throw LargeIntegerExñeption("Empty Polynomial in Pow");
	}

	LargeInteger res("1", base);

	while (b)
	{
		if (b & 1)
		{
			res *= (*this);
		}
		operator*=(*(this));
		b = b >> 1;
	}

	return res;
}



LargeInteger LargeInteger::TranslateTo10()
{
	if (base == 1)
	{
		return LargeInteger(*this);
	}
	
	int base_ = (int)pow(10, base);

	LargeInteger tmp("0", 1);

	for (int i = 0; i < size; ++i)
	{
		tmp += data[i] * (int)pow(base_, i);
	}

	return tmp;
}

LargeInteger LargeInteger::TranslateTo(int k)
{
	int base_ = (int)pow(10, base);
	if (k == base_)
	{
		return LargeInteger(*this);
	}

	LargeInteger num10 = TranslateTo10();

	int sizeOfItem = 0;
	while(k>1)
	{
		++sizeOfItem;
		k /= 10;
	}

	int countOfItems = (int)((double)num10.size / sizeOfItem + 0.5);
	int* tmp_data = new int[countOfItems];
	int tmp_i = 0;

	for (int i = 0; i < num10.size;)
	{
		int current = 0;
		for (int j = 0; j < sizeOfItem && i < num10.size; ++j, ++i)
		{
			current += num10.data[i] * (int)pow(10, j);
		}
		tmp_data[tmp_i] = current;
		++tmp_i;
	}

	delete[] num10.data;
	num10.data = tmp_data;
	num10.size = countOfItems;
	num10.base = sizeOfItem;

	return num10;
}

LargeInteger Factorial(int n)
{
	LargeInteger res("1", 1);

	while (n)
	{
		res *= LargeInteger(n);
		n--;
		//std::cout << n << std::endl;
	}

	return res;
}

LargeInteger BinomialCoeff(int n, int k)
{
	LargeInteger Cn = ("1", 1);

	while (k)
	{
		//std::cout << "<    " << Cn << std::endl;
		Cn = Cn.MulInt(n - k + 1);
		///std::cout << ">=    " << Cn << std::endl;
		Cn /= k;
		//std::cout << "> /   " << Cn << std::endl;
		--k;
	}

	return Cn;
}

LargeInteger Fibonacci(int n)
{
	if (n == 0)
	{
		return LargeInteger("0", 1);
	}

	if (n == 1)
	{
		return LargeInteger("1", 1);
	}

	LargeInteger item1("0", 1);
	LargeInteger item2("1", 1);
	LargeInteger item3("1", 1);

	for (int i = n - 2; i > 0; --i)
	{
		item1 = item2;
		item2 = item3;
		item3 = item1 + item2;
	}
	
	return item3;
}
