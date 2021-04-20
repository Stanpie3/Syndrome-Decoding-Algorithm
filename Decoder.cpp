#include "Decoder.h"
#include <cassert>
#include <cmath>

Decoder::Decoder(): H_(Matrix()), S(Matrix()), G_(Matrix()), n_(0), k_(0) {}

Decoder::Decoder(const Matrix& H, int L): H_(H), L(L), n_(H_.GetNumColumns()), k_(abs(H.GetNumRows() - n_)) {	//Конструктор элемента типа Decoder
	EMatrix I(k_);																							   //По матрице H и степени L
	Matrix Pt = H_.DeConcantenate(k_).first;
	G_ = Concantenate(I, Transpose(Pt));
}

Matrix Decoder::GetG() {
	return G_;
}

Matrix Decoder::GetH() {
	return H_;
}

int Decoder::GetL() {
	return L;
}
int Decoder::GetK() {
	return k_;
}
int Decoder::GetN() {
	return n_;
}

vector<int64_t> Decoder::SetS(const Matrix& message_with_error){	//Функция вычисления синдромной матрицы
	S = H_ * message_with_error;
	return FromBinaryMatrix(S);					//Возвращает синдром над полем 
}

Matrix Decoder::GetS() {
	return S;
}

Matrix ToBinaryMatrix(const vector<int64_t>& v, int length) {	//Функция перехода от вектора чисел над GF(2^L) к их представлению в виде бинарной матрицы
	vector<vector<int>> letters;
	for (int64_t m : v) {
		int64_t m_tmp = m;
		vector<int> letter(length);
		for (int i = length-1; i >= 0; i--) {
			letter[i] = m_tmp % 2;
			m_tmp /= 2;
		}
		letters.push_back(letter);
	}
	Matrix Letters(letters.size(), length, letters);
	return Letters;
}

vector<int64_t> FromBinaryMatrix(const Matrix& M) {		//Функция перехода от бинарной матрицы к вектору чисел над полем GF(2^L)
	vector<int64_t> result;
	for (int row = 0; row < M.GetNumRows(); row++) {
		int64_t letter = 0;
		for (int column = 0; column < M.GetNumColumns(); column++) {
			letter += M.At_c(row, column) * pow(2, M.GetNumColumns()-1-column);
		}
		result.push_back(letter);
	}
	return result;
}

vector<int64_t> Decoder::Encode(const vector<int64_t>& message) { //Кодирование сообщения (использовал для генерации кода)
	if (message.size() == G_.GetNumRows()) {
		Matrix Letters(ToBinaryMatrix(message, L));
		for (int column = k_; column < G_.GetNumColumns(); column++) {
			Matrix Additional_Letter(1, L);
			for (int row = 0; row < G_.GetNumRows(); row++) {
				if (G_.At_c(row, column)) {
					Additional_Letter = Additional_Letter + Letters.GetRow(row);
				}
			}
			vector<int> tmp = Additional_Letter.GetRowVect(0);
			Letters.Push_row(tmp);
		}
		return FromBinaryMatrix(Letters);
	}
	else {
		assert(message.size() == G_.GetNumRows());
	}
}

Matrix Decoder::SDA(const Matrix& message_with_error) {
	vector<int64_t> s = this->SetS(message_with_error);
	Matrix T = Concantenate(S, H_);	//Создаем присоединенную матрицу
	//cout << "Matrix T:\n" << T;
	int r = Rank(S);				//Находим ранг S
	//cout << "Rank of S:\n" << r << endl;
	GaussianBool(T);				//Приводим присоединенную матрицу к форме Гаусса
	//cout << "Matrix T after Gaussian:\n" << T;
	Matrix Bt;				//Создаём матрицу B (до транспонирования)
	int rank_B = 0;
	Matrix Hh = T.DeConcantenate(S.GetNumColumns()).second;
	//cout << "Matrix H' after Deconcantinating:\n" << Hh;
	int size_of_independent = r;
	vector<int> H_indexes;

	for (int column = 0; (column < Hh.GetNumColumns() && rank_B<r); column++) {	//Проходим преобразованную H
		vector<int> Column = Hh.GetColumn_vect(column);
		bool credability = 1;
		for (int i = size_of_independent; i < Column.size() && rank_B<r; i++) {	
			if (Column[i] == 1) {
				credability = 0;			//Если в позиции есть единица, переходим на следующую итерацию цикла
				i = n_;
			}
		}	
		if (credability) {
			for (int j = 0; j < Column.size(); j++) {
				cout << Column[j] << " ";
			}
			cout << endl;
			vector<int> independent_row(Column.begin(), Column.begin()+r);
			Matrix tmp = Bt;	//Проверка линейной независимости
			tmp.Push_row(independent_row);
			cout << "Try of new tmp:\n"<<column<<'\n' << tmp;
			if (Rank(Transpose(tmp)) > rank_B) {
				rank_B++;
				Bt = tmp;
				H_indexes.push_back(column);
			}
			else {
				continue;	//Если рассмотренная колонка зависима, не добавляем её и переходим на следующую итерацию
			}
		}
		else {
			continue;
		}
	}
	//cout << Bt;
	Matrix B_inv = InverseMatrix(Transpose(Bt));
	Matrix s_new = Hh * message_with_error;
	vector<int> s_supp = SupportOfVector(FromBinaryMatrix(s_new));
	Matrix s_sigma;
	for (int element : s_supp) {
		s_sigma.Push_row(s_new.GetRowVect(element));
	}
	Matrix E_sigma = B_inv * s_sigma;
	Matrix Errors;
	int counter = 0;
	for (int i = 0; i < n_; i++) {
		if (find(H_indexes.begin(), H_indexes.end(), i) != H_indexes.end()) {
			Errors.Push_row(E_sigma.GetRowVect(counter));
			counter++;
		}
		else {
			Errors.Push_row(vector<int>(L, 0));
		}
	}
	return (message_with_error + Errors);
}