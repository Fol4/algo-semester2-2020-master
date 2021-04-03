#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

struct segment {
	int64_t start;
	int64_t end;
	int64_t sum;
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

		cur->sum = std::min(cur->left->sum, cur->right->sum);

		return cur;
	}

	int64_t _getMin(int64_t start, int64_t end, segment* cur) {
		if (cur->start == start and cur->end == end) {
			return cur->sum;
		}
		else if (cur->start >= end) {
			return 3*255;
		}
		else if (cur->end <= start) {
			return 3*255;
		}

		int64_t left = 3 * 255;
		int64_t right = 3 * 255;
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

		return std::min(left, right);
	}

	void _increase(int64_t start, int64_t end, int64_t a, int64_t b, segment* cur, int64_t delta) {
		if (cur->start >= b) {
			return;
		}
		else if (cur->end <= a) {
			return;
		}
		else if (a <= cur->start and b >= cur->end) {
			down(cur, delta);
			return;
		}

		_increase(start, end - (end - start) / 2, a, b, cur->left, delta);
		_increase(end - (end - start) / 2, end, a, b, cur->right, delta);

		cur->sum = std::min(cur->left->sum, cur->right->sum);
	}

	void down(segment* cur, int64_t delta) {
		if (cur == nullptr) {
			return;
		}

		cur->sum = delta;

		down(cur->left, delta);
		down(cur->right, delta);
	}

public:
	Colors(std::vector<int64_t>& input) : input(input) {
		root = createTree(0, input.size(), new segment{ 0, input.size() });
	}

	int64_t getMin(int64_t start, int64_t end) {
		return _getMin(start, end, root);
	}

	void increase(int64_t start, int64_t end, int64_t delta) {
		_increase(0, input.size(), start, end, root, delta);
	}
};

int main() {
	std::ifstream inf("input.txt");
	std::ofstream outf("output.txt");
	int64_t n, r, g, b;
	inf >> n;
	std::vector<int64_t> input(n);

	for (auto i = 0; i < n; ++i) {
		inf >> r >> g >> b;
		input[i] = r + g + b;
	}

	Colors res(input);

	int64_t count;
	inf >> count;

	for (auto i = 0; i < count; ++i) {
		int64_t start, end, startO, endO;
		inf >> start >> end >> r >> g >> b >> startO >> endO;
		res.increase(start, end + 1, r + g + b);
		outf << res.getMin(startO, endO + 1) << ' ';
	}
}