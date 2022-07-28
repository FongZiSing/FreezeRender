#include "TopK.hpp"
#include "../Utility/Timer.hpp"

#include <Algorithm/TopK.hpp>
#include <vector>
#include <iostream>
#include <iomanip>
#include <numeric>



namespace Pluto
{
	namespace Algorithm
	{
		void GenerateTestCase(int inNum, std::vector<int>& outData)
		{
			outData.clear();
			outData.reserve(inNum);

			while (inNum--)
			{
				outData.emplace_back(rand());
			}
		}
		// Function to return the
// sign of a number
		int sign(double x)
		{
			if (x < 0)
				return -1;
			if (x > 0)
				return 1;
			return 0;
		}

		// Function to swap
		// two numbers in an array.
		void swap(int arr[], int i, int j)
		{
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}

		int select(int arr[], int left,
			int right, int k)
		{
			while (right > left) {
				if (right - left > 600) {
					// Choosing a small subarray
					// S based on sampling.
					// 600, 0.5 and 0.5
					// are arbitrary constants
					int n = right - left + 1;
					int i = k - left + 1;
					double z = log(n);
					double s = 0.5 * exp(2 * z / 3);
					double sd = 0.5 * sqrt(z * s
						* (n - s) / n)
						* sign(i - n / 2);

					int newLeft = std::max(left,
						(int)(k - i * s / n + sd));

					int newRight = std::min(right,
						(int)(k + (n - i) * s / n
							+ sd));

					select(arr, newLeft, newRight, k);
				}

				// Partition the subarray S[left..right]
				// with arr[k] as pivot
				int t = arr[k];
				int i = left;
				int j = right;
				swap(arr, left, k);
				if (arr[right] > t) {
					swap(arr, left, right);
				}

				while (i < j) {
					swap(arr, i, j);
					i++;
					j--;

					while (arr[i] < t)
						i++;
					while (arr[j] > t)
						j--;
				}

				if (arr[left] == t)
					swap(arr, left, j);
				else {
					j++;
					swap(arr, right, j);
				}

				// Adjust the left and right pointers
				// to select the subarray having k
				if (j <= k)
					left = j + 1;
				if (k <= j)
					right = j - 1;
			}
			return arr[k];
		}


		void Test(FindKthLargest)
		{
			for (int num = 500; num < 100000; num += 500)
			{
				// Random data.
				std::vector<int> data;
				GenerateTestCase(num, data);

				std::vector<int> sorted = data;
				std::sort(sorted.begin(), sorted.end(), std::greater<int>{});

				constexpr int loopmax = 1;
				std::vector<double> accum;
				accum.reserve(loopmax);
				
				std::vector<unsigned char> result;
				result.reserve(loopmax);
				
				for (int loop = 0; loop < loopmax; ++loop)
				{
					std::vector<int> dup = data;
					int randomPivot = rand() % dup.size();

					Timer timer;
					timer.start();
#if 0
					FloydRivestSelect(dup.data(), 0, dup.size() - 1, dup.size() - randomPivot - 1);
					int ret = dup[dup.size() - randomPivot - 1];
#else
					int ret = QuickSelect(dup.data(), dup.size(), randomPivot);
#endif
					timer.stop();
					accum.push_back(timer.total);
					result.push_back(sorted[randomPivot] == ret);
				}

				if (std::find(result.begin(), result.end(), unsigned char(0)) != result.end())
				{
					force_softbreak;
				}

				double t = std::accumulate(accum.begin(), accum.end(), 0.0) / loopmax;
				std::cout << std::setprecision(8) << t << '\n';
			}

		}
	}
}