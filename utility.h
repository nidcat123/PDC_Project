#include <iostream>
#include <string>
#include <string.h>
#include <random>
#include "ompBased.h"
#include "serialTime.h"
#include "uint.h"
int main() {
	std::cout << "uint16 is: " << sizeof(uint16)*8 << " bit" << std::endl;
	const int N = 10000+1;
	std::cout << "N is: " << N-1 << std::endl;
	char *A = new char[N];
	char *B = new char[N];
	char LUT[4] = {'A', 'T', 'C', 'G'};
	//random nummber generator
	std::default_random_engine generator;
	//uniformly distributed values from 0 to 3 representing the number of character in
	LUT
	std::uniform_int_distribution<int> distribution(0, 3);
	std::uniform_real_distribution<double> uniformProb(0.0, 1.0);
	const float pChange = 0.5;
	int priorDistance = 0;
	for (int i = 0; i < N-1; i++) {
		int dice = distribution(generator);
		B[i] = A[i] = LUT[dice];
		if (uniformProb(generator) < pChange) {
		int dice2 = distribution(generator);
		if (dice2 != dice) {
		B[i] = LUT[dice2];
		++priorDistance;
		}
		}
	}
	A[N - 1] = B[N - 1] = '\0';
	if (strlen(A) < strlen(B)) {
	std::swap(A, B);
	}
	printf("computing edit distance\n");
	int d;
	double t1, t2;
	double ST_Time = 0.0, CPPT_Time = 0.0, CPPT2_Time = 0.0, CPPT3_Time = 0.0, OMP_Time
	= 0.0;
	int iterations = 5;
	//ST_Time = 0.0014*iterations;
	for(int i = 0; i < iterations; i++) {
		printf("Iteration n: %i\n", i+1);
		t1 = omp_get_wtime();
		d = editDistanceST(A, B);
		t2 = omp_get_wtime();
		ST_Time += t2 - t1;
		std::cout << "The edit distance is: " << d << "; Computed in (ST): " << t2 - t1 <<
		std::endl;
		t1 = omp_get_wtime();
		d = editDistanceOMP(A, B);
		t2 = omp_get_wtime();
		OMP_Time += t2 - t1;
		std::cout << "The edit distance is: " << d << "; Computed in (OMP): " << t2 - t1 <<
		std::endl;
	}
	std::cout << "ST time " << ST_Time/iterations << std::endl;
	std::cout << "OMP time " << OMP_Time/iterations << " SpeedUp: " << ST_Time/OMP_Time
	<<std::endl;
	return 0;
}
