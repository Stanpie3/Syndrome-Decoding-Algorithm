#include "matrix.h"
#include <stdexcept>
#include <iostream>
#include <tuple>
#include <cassert>

Matrix::Matrix() :num_rows_(0), num_columns_(0) {}

Matrix::Matrix(int num_rows, int num_columns) {
	Reset(num_rows, num_columns);
}

Matrix::Matrix(const Matrix& OtherMatrix) : num_rows_(OtherMatrix.GetNumRows()),
num_columns_(OtherMatrix.GetNumColumns()) {
	Reset(num_rows_, num_columns_);
	for (int row = 0; row < OtherMatrix.GetNumRows(); row++) {
		for (int column = 0; column < OtherMatrix.GetNumColumns(); column++) {
			elements_.at(row).at(column) = OtherMatrix.At_c(row, column);
		}
	}
}

Matrix::Matrix(int num_rows, int num_columns, const vector<vector<int>>& data) :
	num_rows_(num_rows),
		num_columns_(num_columns) {
	Reset(num_rows, num_columns);
	for (int row = 0; row < data.size(); row++) {
		for (int column = 0; column < data[row].size() && column <= num_columns; column++) {
			elements_.at(row).at(column) = data[row][column];
		}
	}
}

EMatrix::EMatrix() : Matrix() {}

EMatrix::EMatrix(int size) : Matrix(size, size) {
	for (int i = 0; i < size; i++) {
		elements_.at(i).at(i) = 1;
	}
}

void Matrix::Reset(int num_rows, int num_columns) {
	assert(num_rows >= 0);
	assert(num_columns >= 0);
	if (num_rows == 0 || num_columns == 0) {
		num_rows = num_columns = 0;
	}
	num_rows_ = num_rows;
	num_columns_ = num_columns;
	elements_.assign(num_rows, vector<int>(num_columns));
}
Matrix Matrix::GetRow(int row)const {
	Matrix M(1, num_columns_);
	for (int column = 0; column < num_columns_; column++) {
		M.At(0, column) = elements_.at(row).at(column);
	}
	return M;
}

vector<int> Matrix::GetRowVect(int row) const {
	return elements_.at(row);
}
Matrix Matrix::GetColumn(int column)const {
	Matrix result(num_rows_, 1);
	for (int i = 0; i < num_rows_; ++i) {
		result.At(i, 0) = this->At_c(i, column);
	}
	return result;
}
vector<int> Matrix::GetColumn_vect(int column)const {
	vector<int> result(num_rows_);
	for (int i = 0; i < num_rows_; ++i) {
		result[i] = elements_.at(i).at(column);
	}
	return result;
}
void Matrix::Push_row(const vector<int>& r) {
	elements_.push_back(r);
	num_rows_++;
	if (num_columns_ == 0) {
		num_columns_ = r.size();
	}
}


void Matrix::Push_column(const Matrix& Col) {
	if (num_rows_ == 0) {
		num_rows_ = Col.GetNumRows();
		for (int row = 0; row < num_rows_; row++) {
			vector<int> v = { Col.At_c(row, 0) };
			elements_.push_back(v);
		}
	}
	else {
		for (int row = 0; row < num_rows_; row++) {
			elements_.at(row).push_back(Col.At_c(row, 0));
		}
	}
	num_columns_++;
}
pair<Matrix, Matrix> Matrix::DeConcantenate(int column) {
	pair<Matrix, Matrix> result;
	for (int row = 0; row < num_rows_; ++row) {
		result.first.Push_row({ elements_.at(row).begin(), elements_.at(row).begin() + column});
		result.second.Push_row({ elements_.at(row).begin() + column, elements_.at(row).end() });
	}
	return result;
}

int& Matrix::At(int row, int column) {
	return elements_.at(row).at(column);
}
int Matrix::At_c(int row, int column) const {
	return elements_.at(row).at(column);
}
int Matrix::GetNumRows() const {
	return num_rows_;
}

int Matrix::GetNumColumns() const {
	return num_columns_;
}

bool operator==(const Matrix& one, const Matrix& two) {
	if (one.GetNumRows() != two.GetNumRows()) {
		return false;
	}

	if (one.GetNumColumns() != two.GetNumColumns()) {
		return false;
	}

	for (int row = 0; row < one.GetNumRows(); ++row) {
		for (int column = 0; column < one.GetNumColumns(); ++column) {
			if (one.At_c(row, column) != two.At_c(row, column)) {
				return false;
			}
		}
	}
	return true;
}

istream& operator>>(istream& in, Matrix& matrix) {
	int rows, columns;
	in >> rows;
	in.ignore(1);
	in >> columns;
	matrix.Reset(rows, columns);
	for (int row = 0; row < rows; ++row) {
		for (int column = 0; column < columns && in; ++column) {
			in >> matrix.At(row, column);
		}
	}
	return in;
}

ostream& operator<<(ostream& out, const Matrix& matrix) {
	out << matrix.GetNumRows() << "x" << matrix.GetNumColumns() << '\n';
	for (int row = 0; row < matrix.GetNumRows(); ++row) {
		for (int column = 0; column < matrix.GetNumColumns(); ++column) {
			if (column > 0) {
				out << " ";
			}
			out << matrix.At_c(row, column);
		}
		out << '\n';
	}
	out << endl;
	return out;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
	if (lhs.GetNumColumns() != rhs.GetNumRows()) {
		throw invalid_argument("\nERROR: (*) The number of Columns in lhs must match the number of Rows in rhs");
	}
	Matrix result(lhs.GetNumRows(), rhs.GetNumColumns());
	for (int row = 0; row < result.GetNumRows(); ++row) {
		for (int column = 0; column < result.GetNumColumns(); ++column) {
			int tmp_res = 0;
			for (int i = 0; i < lhs.GetNumColumns(); i++) {
				tmp_res += lhs.At_c(row, i) * rhs.At_c(i, column);
			}
			result.At(row, column) = tmp_res%2;
		}
	}
	return result;
}
Matrix operator+(const Matrix& one, const Matrix& two) {
	if (one.GetNumRows() != two.GetNumRows()) {
		throw invalid_argument("\nERROR:Mismatched number of rows(+)");
	}

	if (one.GetNumColumns() != two.GetNumColumns()) {
		throw invalid_argument("\nERROR:Mismatched number of columns(+)");
	}

	Matrix result(one.GetNumRows(), one.GetNumColumns());
	for (int row = 0; row < result.GetNumRows(); ++row) {
		for (int column = 0; column < result.GetNumColumns(); ++column) {
			result.At(row, column) = (one.At_c(row, column) + two.At_c(row, column))%2;
		}
	}
	return result;
}

Matrix Transpose(const Matrix& a) {
	Matrix result(a.GetNumColumns(), a.GetNumRows());
	for (int row = 0; row < a.GetNumRows(); row++) {
		for (int column = 0; column < a.GetNumColumns(); column++) {
			result.At(column, row) = a.At_c(row, column);
		}
	}
	return result;
}

Matrix InverseMatrix(const Matrix& m) {
	if (m.GetNumColumns() != m.GetNumRows()) {
		throw invalid_argument("Can't find inverse of non-square matrix");
	}
	if (Rank(m) < m.GetNumRows()) {
		throw invalid_argument("Can't find inverse of matrix with det = 0");
	}
	Matrix M = Concantenate(m, EMatrix(m.GetNumColumns()));
	GaussianBool(M);
	pair<Matrix, Matrix> result = M.DeConcantenate(m.GetNumRows());
	return result.second;
}
Matrix Concantenate(const Matrix& m1, const Matrix& m2) {
	if (m1.GetNumRows() != m2.GetNumRows()) {
		throw invalid_argument("Can not concantenate matrixes of different row nums");
	}
	else {
		Matrix result(m1.GetNumRows(), m1.GetNumColumns() + m2.GetNumColumns());
		for (int row = 0; row < result.GetNumRows(); ++row) {
			for (int column = 0; column < result.GetNumColumns(); ++column) {
				if (column < m1.GetNumColumns()) {
					result.At(row, column) = m1.At_c(row, column);
				}
				else {
					result.At(row, column) = m2.At_c(row, column - m1.GetNumColumns());
				}
			}
		}
		return result;
	}
}

vector<int> SupportOfVector(const vector<int64_t>& v) {
	vector<int> supp;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] != 0) {
			supp.push_back(i);
		}		
	}
	return supp;
}
Matrix VectorFromSupp(vector<int> supp, int size) {
	vector<int> m(size, 0);
	vector<vector<int>> mm;
	if (size >= supp.size()) {
		for (int v : supp) {
			m[v] = 1;
		}
		mm.push_back(m);
		Matrix M(1, size, mm);
	}
	else {
		throw invalid_argument("Vector from support of incorrect size");
	}
}

int GaussianBool(Matrix& mat) {
	int n = mat.GetNumRows();
	int m = mat.GetNumColumns();
	int rank = n;
	for (int i = 0; i < n; i++) {
		if (!mat.At_c(i, i)) {
			int j;
			for (j = i + 1; j < n && !mat.At_c(j, i); j++);
			if (j == n) {
				rank--;
				continue;
			}
			else
				for (int k = i; k < m; k++) {
					bool t = mat.At_c(i, k);
					mat.At(i, k) = mat.At_c(j, k);
					mat.At(j, k) = t;
				}
		}
		for (int j = i + 1; j < n; j++) {
			if (mat.At(j, i)) {
				for (int k = i; k < m; k++)
					mat.At(j, k) = abs(mat.At_c(j, k) - mat.At_c(i, k)) % 2;
			}
		}
	}
	return rank;
}

int Rank(const Matrix& matrix) {
	Matrix mat = matrix;
	int n = mat.GetNumRows();
	int m = mat.GetNumColumns();
	int rank = (n<m) ? n : m;
	if (n <= m) {
		for (int i = 0; i < n; i++) {
			if (!mat.At_c(i, i)) {
				int j;
				for (j = i + 1; j < n && !mat.At_c(j, i); j++);
				if (j == n) {
					rank--;
					continue;
				}
				else
					for (int k = i; k < m; k++) {
						bool t = mat.At_c(i, k);
						mat.At(i, k) = mat.At_c(j, k);
						mat.At(j, k) = t;
					}
			}
			for (int j = i + 1; j < n; j++) {
				if (mat.At(j, i)) {
					for (int k = i; k < m; k++)
						mat.At(j, k) = abs(mat.At_c(j, k) - mat.At_c(i, k)) % 2;
				}
			}
		}
	}
	else {
		for (int i = 0; i < m; i++) {
			if (!mat.At_c(i, i)) {
				int j;
				for (j = i + 1; j < n && !mat.At_c(j, i); j++);
				if (j == n) {
					rank--;
					continue;
				}
				else
					for (int k = i; k < m; k++) {
						bool t = mat.At_c(i, k);
						mat.At(i, k) = mat.At_c(j, k);
						mat.At(j, k) = t;
					}
			}
			for (int j = i + 1; j < n; j++) {
				if (mat.At(j, i)) {
					for (int k = i; k < m; k++)
						mat.At(j, k) = abs(mat.At_c(j, k) - mat.At_c(i, k)) % 2;
				}
			}
		}
	}
	return rank;
}

int Weight(const Matrix& m) {
	int result=0;
	for (int row = 0; row < m.GetNumRows(); row++) {
		for (int column = 0; column < m.GetNumColumns(); column++) {
			result += m.At_c(row, column);
		}
	}
	return result;
}

