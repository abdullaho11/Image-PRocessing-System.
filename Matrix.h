#include<iostream>
#include <stdexcept>
using namespace std;
template<typename T>
class Matrix {
private:
	int row;
	int col;
	T* arr;
public:
	Matrix(int r, int c) {
		row = r;
		col = c;
		if (row > 0 && col > 0) {
			arr = new T[row * col];
		}
		else
			arr = nullptr;
	}
	Matrix(const Matrix& other) {
		row = other.row;
		col = other.col;
		arr = new T[row * col];
		for (int i = 0; i < row * col; i++)
			arr[i] = other.arr[i];
	}
	~Matrix() {
		delete[] arr;
	}
	T& operator()(int i, int j) {
		if (i < 0 || i >= row || j < 0 || j >= col) {
			throw out_of_range("Index out of range");
		}
		return arr[i * col + j];
	}

	const T& operator()(int i, int j) const {
		if (i < 0 || i >= row || j < 0 || j >= col) {
			throw out_of_range("Index out of range");
		}
		return arr[i * col + j];
	}


	Matrix operator+(const Matrix& arr2) const {
		if (row != arr2.row || col != arr2.col) {
			throw invalid_argument("Matrix dimensions must match");
		}
		Matrix Result(row, col);
		for (int i = 0;i < row * col;i++) {
			Result.arr[i] = arr[i] + arr2.arr[i];
		}
		return Result;
	}
	Matrix operator-(const Matrix& arr2) const {
		if (row != arr2.row || col != arr2.col) {
			throw invalid_argument("Matrix dimensions must match");
		}
		Matrix Result(row, col);
		for (int i = 0;i < row * col;i++) {
			Result.arr[i] = arr[i] - arr2.arr[i];
		}
		return Result;
	}
	Matrix operator*(T scalar) const
	{
		Matrix Result(row, col);
		for (int i = 0; i < row * col; i++) {
			Result.arr[i] = arr[i] * scalar;
		}
		return Result;
	}
	Matrix operator*(const Matrix& arr2) const {
		if (col != arr2.row) {
			throw invalid_argument("Number of columns of the first matrix must match the number of rows of the second matrix");
		}
		Matrix Result(row, arr2.col);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < arr2.col; j++) {
				Result.arr[i * arr2.col + j] = 0;
				for (int k = 0; k < col; k++) {
					Result.arr[i * arr2.col + j] += arr[i * col + k] * arr2.arr[k * arr2.col + j];
				}
			}
		}
		return Result;
	}
	Matrix& operator=(const Matrix& other) {
		if (this != &other) {
			delete[] arr;
			row = other.row;
			col = other.col;
			arr = new T[row * col];
			for (int i = 0; i < row * col; i++)
				arr[i] = other.arr[i];
		}
		return *this;
	}
	Matrix transpose() {
		Matrix result(col, row);
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				result(j, i) = (*this)(i, j);
			}
		}

		return result;
	}
	int getrow() const { return row; }
	int getcol() const { return col; }
};