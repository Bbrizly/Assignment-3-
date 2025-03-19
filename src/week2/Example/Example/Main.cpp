#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
	static const int SIZE = 15000;
	static const int ITERATION_COUNT = 10;

	int* values = new int[SIZE * SIZE];
	memset(values, 0, sizeof(int) * SIZE * SIZE);
	
	for (int iteration = 0; iteration < ITERATION_COUNT; ++iteration)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		int sum = 0;
		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				sum += values[(i * SIZE) + j];	//sum += values[i][j];
				//sum += values[(j * SIZE) + i];		//sum += values[j][i];
			}
		}

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);
		std::cout << "Done in [" << duration.count() << "] milliseconds" << std::endl;
	}

	return 0;
}
