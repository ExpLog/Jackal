#include <iostream>
#include "decomposition.h"
#include "matrix.h"

int main() {
	using namespace std;

	cout << "Testing printing and constructors with the Identity 1x1, 2x2 and 3x3." << endl;

	Matrix id_1_1(1, 1, 0.0);
	cout << "Constructed zero 1x1 matrix!" << endl;
	id_1_1(0,0) = 1.0;
	cout << "Constructed Id 1x1!" << endl;
	cout << id_1_1 << endl;

	Matrix id_2_2(2, 2, 0.0);
	for (size_t i = 0; i < 2u; ++i){
		id_2_2(i,i) = 1.0;
	}
	cout << "Constructed Id 2x2!" << endl;
	cout << id_2_2 << endl;

	Matrix id_3_3(3, 3, 0.0);
	for (size_t i = 0; i < 3u; ++i){
		id_3_3(i,i) = 1.0;
	}
	cout << "Constructed Id 3x3!" << endl;
	cout << id_3_3 << endl;


	return 1;
}