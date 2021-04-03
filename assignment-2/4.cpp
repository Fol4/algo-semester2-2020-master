#include <vector>
#include <iostream>
#include <string>

class Graph {
	std::vector<std::vector<uint64_t>> graph;
	uint64_t size;

	void make_matrix() {
		for (auto i = 1; i < size; ++i) {
			for (auto j = 0; j < i; ++j) {

				if (graph[i][j]) {
					for (auto k = 0; k < size; ++k) {
						if (graph[i][k] == 1 or graph[j][k] == 1) {
							graph[i][k] = 1;
						}
					}
				}
			}
		}
		for (auto i = 0; i < size; ++i) {
			for (auto j = i + 1; j < size; ++j) {

				if (graph[i][j]) {
					for (auto k = 0; k < size; ++k) {
						if (graph[i][k] == 1 or graph[j][k] == 1) {
							graph[i][k] = 1;
						}
					}
				}
			}
		}
	}

public:
	Graph(uint64_t _size): size(_size), graph(_size, std::vector<uint64_t>(_size, 0)){}

	void AddLink(uint64_t i, uint64_t j, uint64_t val) {
		graph[i][j] = val;
	}

	void output() {
		make_matrix();

		for (auto node : graph) {
			for (auto link : node) {
				std::cout << link;
			}
			std::cout << std::endl;
		}
	}
};



int main() {
	uint64_t n;
	char str;
	std::cin >> n;
	Graph graph(n);

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			std::cin >> str;
			graph.AddLink(i, j, str - '0');
		}
	}

	graph.output();
}