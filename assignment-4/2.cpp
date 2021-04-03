#include <iostream>
#include <vector>
#include <algorithm>

struct segment {
	int64_t start;
	int64_t end;
	int64_t sum = 0;
	segment* left = nullptr;
	segment* right = nullptr;
};

class Colors {
	segment* root;
	std::vector<int64_t> input;

	segment* createTree(int64_t start, int64_t end, segment* cur) {
		if (start == end - 1) {
			cur->sum = input[start];
			return cur;
		}

		cur->left = createTree(start, end - (end - start) / 2, new segment{ start, end - (end - start) / 2 });
		cur->right = createTree(end - (end - start) / 2, end, new segment{ end - (end - start) / 2, end });

		cur->sum = std::max(cur->left->sum, cur->right->sum);

		return cur;
	}

	int64_t _getMin(int64_t start, int64_t end, segment* cur) {
		if (cur->start == start and cur->end == end) {
			return cur->sum;
		}
		else if (cur->start >= end) {
			return 0;
		}
		else if (cur->end <= start) {
			return 0;
		}

		int64_t left = 0;
		int64_t right = 0;
		if (cur->left->start <= start and cur->left->end >= end) {
			left = _getMin(start, end, cur->left);
		}
		else if (cur->right->start <= start and cur->right->end >= end) {
			right = _getMin(start, end, cur->right);
		}
		else {
			left = _getMin(start, cur->left->end, cur->left);
			right = _getMin(cur->right->start, end, cur->right);
		}

		return std::max(left, right);
	}

	void _increase(int64_t start, int64_t end, int64_t a, int64_t b, segment* cur, int64_t delta) {
		if (cur->start >= b) {
			return;
		}
		else if (cur->end <= a) {
			return;
		}
		else if (start == end - 1) {
			cur->sum += delta;
			return;
		}

		_increase(start, end - (end - start) / 2, a, b, cur->left, delta);
		_increase(end - (end - start) / 2, end, a, b, cur->right, delta);

		cur->sum = std::max(cur->left->sum, cur->right->sum);
	}
	 
public:
	Colors(std::vector<int64_t> input): input(input) {
		root = createTree(0, input.size(), new segment{ 0, input.size() });
	}

	int64_t getMax(int64_t start, int64_t end) {
		return _getMin(start, end, root);
	}

	void increase(int64_t start, int64_t end, int64_t delta) {
		_increase(0, input.size(), start, end, root, delta);
	}
};

int main() {
	int64_t n;
	std::cin >> n;
	std::vector<int64_t> input(n - 1);

	for (auto i = 0; i < n - 1; ++i) {
		std::cin >> input[i];
	}

	Colors res(input);

	int64_t capacity, count;
	std::cin >> capacity >> count;

	for (auto i = 0; i < count; ++i) {
		int64_t start, end, delta;
		std::cin >> start >> end >> delta;

		if (res.getMax(start, end) + delta > capacity) {
			std::cout << i << ' ';
		}
		else {
			res.increase(start, end, delta);
		}
	}
}