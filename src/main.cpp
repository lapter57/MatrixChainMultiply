#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <chrono>
#include "MatrixDef.h"
#include <iomanip>
using namespace std;

int randomNumber(unsigned int l = 1, unsigned int r = 100) { return (l + (rand() % (r - l + 1))); }
int randomFillMatrix() { return (1 + rand() % 2); }

Matrix<unsigned long long>* matrixChainOrder(const vector<unsigned int>& sizeMat) {
    unsigned int n = sizeMat.size();
    Matrix<unsigned long long> M(n, n);
    Matrix<unsigned long long> *S;
    S = new Matrix<unsigned long long>(n - 1, n - 1);

    for (register int l = 1; l < n; l++) {
        for (register int i = 1; i < n - l; i++) {
            int j = i + l;
            M(i - 1, j - 1) = numeric_limits<unsigned long long>::max();
            for (register int k = i; k <= j - 1; k++) {
                int q = M(i - 1, k - 1) + M(k, j - 1) + sizeMat[i-1] * sizeMat[k] * sizeMat[j];
                if (q < M(i - 1, j - 1)) {
                    M(i - 1, j - 1) = q;
                    (*S)(i - 1, j - 1) = k-1;
                }
            }
        }
    }
    return S;
}

template <class T>
Matrix<T> matrixChainMultiply(const vector<Matrix<T>*>& matrix, const Matrix<T> &S, int row, int col, ostream& out) {
    if (row == col) {
        out << "A" << row << " ";
        return *matrix[row];
    }
    out << "( ";
    Matrix<T> A(matrixChainMultiply(matrix, S, row, S(row, col), out));
    Matrix<T> B(matrixChainMultiply(matrix, S, S(row, col) + 1, col, out));
    out << ")";
    return A*B;
}


void task() {
    using namespace std::chrono;
    ifstream in;
    ofstream out;
    ofstream outData;
    in.open("input_1.txt");
    out.open("output_1.txt");
    outData.open("data_M10.txt");
    unsigned int l, r, n;
    in >> l >> r >> n;

    vector<unsigned int> numMat(n);
    for (register int i = 0; i < n; i++) {
        in >> numMat[i];
    }

    for (int r = 10; r <= 100; r += 10) {
        outData << "M = " << r << endl << endl;

        vector<Matrix<unsigned long long>*> vec_mat;
        for (int i = 0; i < n; i++) {
            vector<unsigned int> sizeMat;
            sizeMat.resize(numMat[i] + 1);
            for (auto && el : sizeMat) {
                el = randomNumber(l, r);
            }
            int k = 0;
            Matrix<unsigned long long> *matrix;
            for (register int j = 0; j < numMat[i]; j++) {
                matrix = new Matrix<unsigned long long>(sizeMat[k], sizeMat[k + 1]);
                matrix->randomGenerate(&randomFillMatrix);
                //out << *matrix << endl;
                vec_mat.push_back(matrix);
                k++;
            }
            out << endl;

            auto tStart = high_resolution_clock::now();
            Matrix<unsigned long long> *optimalCost = matrixChainOrder(sizeMat);
            Matrix<unsigned long long> productMatrixChain = matrixChainMultiply(vec_mat, *optimalCost, 0, numMat[i] - 1, out);
            auto tEnd = high_resolution_clock::now();
            auto time1 = duration_cast<duration<double, milli>>(tEnd - tStart);
            out << endl << "Number Of Matrix: " << numMat[i] << endl;
            out << "product Of The Matrix Chain (time: " << time1.count() << " ): " << endl;
            //out << productMatrixChain;

            tStart = high_resolution_clock::now();
            Matrix<unsigned long long> product = *vec_mat[0] * *vec_mat[1];
            for (int i = 2; i < vec_mat.size(); i++) {
                product *= *vec_mat[i];
            }
            tEnd = high_resolution_clock::now();
            auto time2 = duration_cast<duration<double, milli>>(tEnd - tStart);
            out << endl << "Number Of Matrix: " << numMat[i] << endl;
            out << "product Of The Matrix Chain (time: " << time2.count() << " ): " << endl;
            //out << product;

            outData << left << setw(5) << numMat[i] << setw(10) << time1.count() << time2.count() << endl;

            sizeMat.clear();
            vec_mat.clear();
        }
        outData << endl << endl;
    }


    in.close();
    out.close();


}

int main() {
    srand((unsigned)time(0));
    task();
    return 0;
}