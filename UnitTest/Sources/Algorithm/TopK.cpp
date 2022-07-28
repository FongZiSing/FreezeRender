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
#if 1
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