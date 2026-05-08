#include <iostream>
#include "Matrix.h"
#include <fstream>
#include <chrono>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Lab1 Slipenchuk Nikita 6212-100503D\n";
        cout << "Running with " << size << " MPI processes\n";
    }

    int n = 2000;

    if (rank == 0) {
        ofstream infile("input.txt");
        Matrix<int> lhs(n, n, -100, 100);
        infile << n << "\n";
        infile << lhs << "\n";
        Matrix<int> rhs(n, n, -100, 100);
        infile << rhs;
        infile.close();
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    Matrix<int> lhs(n, n, 0);
    Matrix<int> rhs(n, n, 0);

    if (rank == 0) {
        ifstream infile("input.txt");
        int read_n;
        infile >> read_n;

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                int val;
                infile >> val;
                lhs(i, j) = val;
            }
        }

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                int val;
                infile >> val;
                rhs(i, j) = val;
            }
        }
        infile.close();
    }

    MPI_Bcast(lhs.data(), n * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(rhs.data(), n * n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    auto start = std::chrono::steady_clock::now();

    Matrix<int> result = multiply_matrix_parallel(lhs, rhs, rank, size);

    auto end = std::chrono::steady_clock::now();

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ofstream outfile("output.txt");
        outfile << result;
        outfile << "time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
        outfile << "size: " << n * n << "\n";
        outfile << "processes: " << size << "\n";
        outfile.close();

        cout << "Result saved to output.txt\n";

        int code = system("python check.py");
        if (code == 1) {
            cout << "correct\n";
        }
        else {
            cout << "uncorrect\n";
        }
    }

    MPI_Finalize();
    return 0;
}