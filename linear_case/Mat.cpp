#include "Mat.h"
#include<iostream>
#include<algorithm>

// -- Construtores 
Mat::Mat()
{
	_rows = 0; _cols = 0;
}

Mat::Mat(size_t m, size_t n)
{
	_rows = m; _cols = n; _dados = new double[m * n];
	std::fill_n(_dados, m * n, 0.0);
}

// -- Construtor Cópia
Mat::Mat(const Mat& m)
{
	_cols = m._cols;
	_rows = m._rows;
	size_t dim = _rows * _cols;
	_dados = new double[dim];
	memcpy(_dados, m._dados, dim * sizeof(double));
}

// -- Construtor por Array 
Mat::Mat(size_t m, size_t n, double* ptr)
{
	_rows = m;
	_cols = n;
	size_t dim = m * n;
	_dados = new double[dim];
	memcpy(_dados, ptr, dim * sizeof(double));
}

// -- Destrutor 
Mat::~Mat()
{
	delete[] _dados;
}

// -- Transporta 
Mat Mat::transp()
{
	Mat result(_cols, _rows);
	for (size_t i = 0; i < result._rows; i++)
		for (size_t j = 0; j < result._cols; j++)
			result(i, j) = *(_dados + j * _cols + i);

	return result;
}

// -- Imprimir 
void Mat::show()
{
	for (size_t i = 0; i < _rows; i++) {
		for (size_t j = 0; j < _cols; j++)
			std::cout << *(_dados + i * _cols + j) << " ";
		std::cout << "\n";
	}
}

double Mat::getMaxCol(size_t col)
{
	double max = DBL_MIN;
	for (size_t i = 0; i < _rows; i++)
		max = maximo(max, this->operator()(i, col));

	return max;
}

double Mat::getMinCol(size_t col)
{
	double min = DBL_MAX;
	for (size_t i = 0; i < _rows; i++)
		min = minimo(min, this->operator()(i, col));

	return min;
}

void Mat::applyFunction(Function f)
{
	for (size_t i = 0; i < _rows; i++)
		for (size_t j = 0; j < _cols; j++)
			this->operator()(i, j) = f(this->operator()(i, j));
}

//--------------
// -- Overloads 
//--------------
Mat& Mat::operator=(const Mat& m)
{
	if (this != &m) {
		_rows = m._rows;
		_cols = m._cols;
		size_t dim = _rows * _cols;
		_dados = new double[dim];
		memcpy(_dados, m._dados, dim * sizeof(double));
	}
	return *this;
}


Mat Mat::operator+(const double& b)
{
	Mat result(_rows, _cols);
	for (size_t i = 0; i < result._rows; i++)
		for (size_t j = 0; j < result._cols; j++)
			result(i, j) = this->operator()(i, j) + b;

	return result;

}


Mat Mat::operator*(const Mat& m)
{
	Mat result(_rows, m._cols);
	for (size_t i = 0; i < result._rows; i++)
		for (size_t j = 0; j < result._cols; j++)
			for (size_t k = 0; k < _cols; k++)
				result(i, j) += this->operator()(i, k) * m(k, j);

	return result;

}
