#include <iostream>
#include "Matrix.h"
#include "Decoder.h"
#include <fstream>

using namespace std;

int main()
{
	try {
		ifstream input("data.txt");
		if (input.is_open()) {
			Matrix H;
			input >> H;
			Decoder D(H, 32);

			vector<int64_t> message;
			for (int i = 0; i < D.GetK(); i++) {
				int64_t tmp;
				input >> tmp;
				message.push_back(tmp);
			}
			vector<int64_t> error;
			for (int i = 0; i < D.GetN(); i++) {
				int64_t tmp;
				input >> tmp;
				error.push_back(tmp);
			}

			vector<int64_t> code =  D.Encode(message);			
			Matrix message_with_error = ToBinaryMatrix(code, D.GetL()) + ToBinaryMatrix(error, D.GetL());
			Matrix C = D.SDA(message_with_error);

			for (int64_t c : FromBinaryMatrix(C)) {
				cout << c << " ";
			}
		}
		else {
			cout << "Can't connect data file";
		}
	}
	catch (exception & ex) {
		cout << ex.what() << endl;
	}
	return 0;
}
