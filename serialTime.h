#ifndef OMP_BASED
#define OMP_BASED
#include <iostream>
#include <string>
#include <omp.h>
#include <unistd.h>
#include "utility.h"
#include "uint.h"
#include <unistd.h>
const int MAX_NUM_THREADS = 12;
const uint TW = 512;
void computeSubMatrix(uint I, uint J, const int M, const int N, const char *x, const
char *y, uint16 *D) {
	uint M_ = M + 1;
	uint N_ = N + 1;
	I = I * TW + 1;
	J = J * TW + 1;
	for (int i = I; i < N_ && i < I + TW; i++) {
		for (int j = J; j < M_ && j < J + TW; j++) {
			if (x[i - 1] != y[j - 1]) {
			uint16 k = minimum(D[i * M_ + j - 1], //insertion
			D[(i - 1) * M_ + j], //insertion
			D[(i - 1) * M_ + j - 1]); //substitution
			D[i * M_ + j] = k + 1;
			} 
			else {
			D[i * M_ + j] = D[(i - 1) * M_ + j - 1];
			}
	}
	}
}
int editDistanceOMP(const char *x, const char *y) {
const uint M = strlen(x);
const uint N = strlen(y);
const uint M_ = M + 1;
const uint N_ = N + 1;
//std::vector<uint16> D(M_*N_);
const uint dim = (uint)M_*(uint)N_;
uint16 * D = new uint16[dim];
//D.resize(M_ * N_);
uint i, j;
for (i = 0; i < M_; i++)
D[i] = i;
for (j = 1; j < N_; j++)
D[j * M_] = j;
////////////////
int M_Tiles = ceil((float) M / TW);
int N_Tiles = ceil((float) N / TW);
const int DStart = -M_Tiles + 1;
const int DFinish = N_Tiles;
const int TILE_WIDTH = TW;
const int TOTAL_TILES = M_Tiles * N_Tiles;
float t1, t2;
#pragma omp parallel num_threads(MAX_NUM_THREADS)
{
#pragma omp master
{
for (int d = DStart; d < DFinish; d++) {
const int iMax = std::min(M_Tiles + d, N_Tiles);
const int iMin = std::max(d, 0);
for (i = iMin; i < iMax; i++) {
#pragma omp task
{
j = M_Tiles - i + d - 1;
computeSubMatrix(i, j, M, N, x, y, &D[0]);
}
}
#pragma omp taskwait
}
}
}
//b.count_down_and_wait();
int distance = D[N * M_ + M];
delete[] D;
return distance;
}
#endif
