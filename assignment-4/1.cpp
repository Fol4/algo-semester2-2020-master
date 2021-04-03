#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class R2Q {
	uint64_t size;
	std::vector<int> input;
	std::vector<std::vector<std::pair<int, int>>> sparseTable;

	std::pair<int, int> _min(std::pair<int, int> first, std::pair<int, int> second) {
		std::vector<int> intersection{ first.first, first.second, second.first, second.second };

		std::sort(intersection.begin(), intersection.end());

		std::pair<int, int> m{ intersection[0], intersection[0] };

		for (auto i = 1; i < 4; ++i) {
			if (intersection[i] != m.first) {
				m.second = intersection[i];
				break;
			}
		}

		return m;
	}

public:
	R2Q(std::vector<int>& input) : input(input), size(input.size()), sparseTable((uint64_t)(ceil(log2(size))), std::vector<std::pair<int, int >>(size, { 0, 0 })) {
		for (auto i = 0; i < size; ++i) {
			sparseTable[0][i] = { input[i], input[i] };
		}

		for (auto i = 1; i < sparseTable.size(); ++i) {
			for (auto j = 0; j < size - (1 << i) + 1; ++j) {
				sparseTable[i][j] = _min(sparseTable[i - 1][j], sparseTable[i - 1][j + (1 << (i - 1))]);
			}
		}
	}

	int ss(uint64_t start, uint64_t end) {
		uint64_t str = (uint64_t)(ceil(log2(end - start + 1))) - 1;

		return _min(sparseTable[str][start], sparseTable[str][end - (1 << str) + 1]).second;
	}
};

int main() {
	uint64_t n, k;
	std::cin >> n >> k;

	std::vector<int> input(n);

	for (auto i = 0; i < n; ++i) {
		std::cin >> input[i];
	}

	R2Q r2q(input);

	for (auto i = 0; i < k; ++i) {
		uint64_t start, end;

		std::cin >> start >> end;
		std::cout << r2q.ss(start - 1, end - 1) << std::endl;
	}
}