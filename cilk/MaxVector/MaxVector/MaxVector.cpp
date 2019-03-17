// MaxVector.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <time.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <Windows.h>
#define n 10
int main(int argc, char* argv[])
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;
	// Start time
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&t1);
	srand(time(0));
	// Use 4 workers so stealing will occur, even on a single core system
	__cilkrts_set_param("nworkers", "2");
	cilk::reducer< cilk::op_max<int> > rm;
	int vector[n];
	cilk_for(int i = 0; i < n; i++)
	{
		vector[i] = rand() % 10;
	}
	cilk_for(int i = 0; i < n; i++)
	{
		printf("vector[%u] = %i\n", i, vector[i]);
		rm->calc_max(vector[i]);
	}
	// Print out the result
	printf("Maximum value = %i\n", rm.get_value());
	// Stop time
	QueryPerformanceCounter(&t2);
	double elapsed = 1000.0*(t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
	std::cout << "Running time: " << elapsed << " ms.n" << std::endl;
	system("pause");
	return 0;
}
