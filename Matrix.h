#pragma once
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
class Matrix {
private:
	int num_rows_;
	int num_columns_;		//Параметры матрицы
protected:
	vector<vector<int>> elements_; //Контейнер, хранящий матрицу
public:
	Matrix();
	Matrix(int num_rows, int num_columns);
	Matrix(int num_rows, int num_columns, const vector<vector<int>>& data);
	Matrix(const Matrix& OtherMatrix);
	void Reset(int num_rows, int num_columns);
	virtual int& At(int row, int column);
	int At_c(int row, int column) const;
	Matrix GetRow(int row)const;
	vector<int> GetRowVect(int row)const;
	Matrix GetColumn(int column)const;
	vector<int> GetColumn_vect(int column)const;
	void Push_row(const vector<int>& r);
	void Push_column(const Matrix& Col);
	pair<Matrix, Matrix> DeConcantenate(int column);
	int GetNumRows() const;
	int GetNumColumns() const;
};
class EMatrix : public Matrix {
public:
	EMatrix();
	EMatrix(int n);
};
istream& operator>>(istream& in, Matrix& matrix);
ostream& operator<<(ostream& out, const Matrix& matrix);
Matrix operator+(const Matrix& one, const Matrix& two);
vector<int> SupportOfVector(const vector<int64_t>& m);
Matrix VectorFromSupp(vector<int> supp, int size);
int GaussianBool(Matrix& m);
Matrix Transpose(const Matrix& m);
Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Matrix Concantenate(const Matrix& m1, const Matrix& m2);
Matrix InverseMatrix(const Matrix& m);
int Weight(const Matrix& m);
int Rank(const Matrix& matrix);