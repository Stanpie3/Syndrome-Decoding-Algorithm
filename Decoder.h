#pragma once
#include "Matrix.h"

class Decoder {
public:
	Decoder();
	Decoder(const Matrix& H, int L);
	Matrix GetG();
	Matrix GetH();
	Matrix GetS();
	vector<int64_t> SetS(const Matrix& message_with_error);
	vector<int64_t> Encode(const vector<int64_t>& message);
	Matrix SDA(const Matrix& message_with_error);
	int GetL();
	int GetK();
	int GetN();
private:
	Matrix G_;
	Matrix H_;
	Matrix S;
	int n_;
	int k_;
	int L=0;
};
Matrix ToBinaryMatrix(const vector<int64_t>& v, int length);
vector<int64_t> FromBinaryMatrix(const Matrix& M);