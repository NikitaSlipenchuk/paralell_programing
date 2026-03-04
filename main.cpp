#include <iostream>
#include "Matrix.h"
#include <fstream>
#include<chrono>

using namespace std;

int main() {
	cout << "Lab1 Slipenchuk Nikita 6212-100503D";
	int n = 2000;
	ofstream infile,outfile ;
	infile.open("input.txt");
	outfile.open("output.txt");
	Matrix<int>lhs(n, n, -100, 100);
	infile << n<<"\n";
	infile << lhs<<"\n";
	Matrix<int>rhs(n, n, -100, 100);
	infile << rhs;
	auto start = std::chrono::steady_clock::now();
	lhs = lhs * rhs;
	auto end = std::chrono::steady_clock::now();
	outfile << lhs;
	outfile << "time:"<<end-start<<"\n";
	outfile << "size:" << n*n;
	int code = system("python check.py");
	if (code == 1) {
		cout << "correct";
	}
	else {
		cout << "uncorrect";
	}
}

