#pragma once

typedef double (*Function)(double);

class Mat
{
private:
    size_t _rows;
    size_t _cols;
    double* _dados = nullptr;

public:
    // -- Construtores -- 
    Mat();
    Mat(size_t m, size_t n);
    ~Mat();
    Mat(const Mat& m);// -- C�pia
    Mat(size_t m, size_t n, double* ptr);

    // -- M�todos Triviais --
    inline size_t rows() const{ return _rows; }
    inline size_t cols() const{ return _cols; }
    inline double* data() { return _dados; }
    Mat transp();// transposta 
    void show();// imprime matriz 
    double getMaxCol(size_t col);// valor m�x. da col 
    double getMinCol(size_t col); // valor m�n. da col
    void applyFunction(Function f);

    //-- Overload --
    inline double& operator()(size_t i, size_t j) {
        return _dados[i * _cols + j];
    }
    inline double operator()(size_t i, size_t j)const {
        return *(_dados + i * _cols + j);
    }
    inline double& operator()(size_t i) {
        return _dados[i * _cols];
    }
    inline double operator()(size_t i)const {
        return *(_dados + i * _cols);
    }

    Mat& operator=(const Mat& m);
    Mat operator+(const double& b);
    Mat operator*(const Mat& m);

private:
    inline double maximo(double x, double y) { return (x > y) ? x : y; }
    inline double minimo(double x, double y) { return (x < y) ? x : y; }

};


