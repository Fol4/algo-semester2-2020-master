#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ctime>

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

enum class ColorN {
	white, grey, black
};

class Node;

struct Link {
	int64_t val;
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	ColorN color;
	std::vector<Node*> NodesIn;
	std::vector<Node*> NodesOut;

public:
	Node(uint64_t number) : number(number), color(ColorN::white) {}

	void AddNodeIn(Node* link) {
		NodesIn.push_back(link);
	}

	void AddNodeOut(Node* link) {
		NodesOut.push_back(link);
	}

	void ChangeColor(ColorN colorl) {
		color = colorl;
	}

	uint64_t GetNumber() {
		return number;
	}

	ColorN GetColor() {
		return color;
	}

	std::vector<Node*>& GetNodesIn() {
		std::sort(NodesIn.begin(), NodesIn.end(), [](Node* first, Node* second) {
			if (first->GetNumber() > second->GetNumber()) {
				return false;
			}
			return true;
			});
		return NodesIn;
	}

	std::vector<Node*>& GetNodesOut() {
		std::sort(NodesOut.begin(), NodesOut.end(), [](Node* first, Node* second) {
			if (first->GetNumber() > second->GetNumber()) {
				return false;
			}
			return true;
			});
		return NodesOut;
	}
};

std::vector<Node*> intersection(std::vector<Node*> v1, std::vector<Node*> v2)
{
	uint64_t first1 = 0;
	uint64_t first2 = 0;
	uint64_t last1 = v1.size();
	uint64_t last2 = v2.size();
	std::vector<Node*> out;

	while (first1 != last1 && first2 != last2) {
		if (v1[first1]->GetNumber() != v2[first2]->GetNumber()) {
			++first1;
		}
		else {
			out.push_back(v1[first1]);
			++first2;
		}
	}
	return out;
}

class Graph {
	std::vector<Node*> nodes;
	std::vector<Link*> links;
	uint64_t size;

public:
	Graph(uint64_t size) : size(size) {
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		if (in != out) {
			Link* NewLink = new Link(val, nodes[in], nodes[out]);

			nodes[in]->AddNodeOut(nodes[out]);
			nodes[out]->AddNodeIn(nodes[in]);

			links.push_back(NewLink);
		}
	}

	Graph reverse() {
		Graph revGraph(size);

		for (auto link : links) {
			revGraph.AddLink(link->node2->GetNumber(), link->node1->GetNumber(), link->val);
		}

		return revGraph;
	}

	std::vector<Node*> dfs(uint64_t number) {
		std::stack<std::pair<Node*, uint64_t>> deep;
		std::vector<Node*> result;

		if (nodes[number]->GetColor() == ColorN::black) {
			return result;
		}

		nodes[number]->ChangeColor(ColorN::black);
		deep.push({ nodes[number], 0 });

		while (!deep.empty()) {
			std::vector<Node*> children = deep.top().first->GetNodesOut();
			std::pair<Node*, uint64_t> cur = deep.top();

			for (auto i = cur.second; i < children.size(); ++i) {
				if (children[i]->GetColor() == ColorN::white) {
					deep.top().second = i + 1;
					children[i]->ChangeColor(ColorN::black);
					deep.push({ children[i], 0 });
					break;
				}
			}

			if (cur.first == deep.top().first) {
				result.push_back(deep.top().first);
				deep.pop();
			}
		}

		return result;
	}

	std::vector<Node*> TopologicalSort() {
		std::vector<Node*> answer;

		for (auto node : nodes) {
			std::vector<Node*> result = dfs(node->GetNumber());

			if (!result.empty()) {
				answer.insert(answer.end(), result.begin(), result.end());
			}
		}

		std::reverse(answer.begin(), answer.end());

		return answer;
	}

	std::vector<std::vector<Node*>>  findSCC() {
		std::vector<std::vector<Node*>> SCC;

		Graph revGraph = reverse();
		std::vector<Node*> sorted = TopologicalSort();

		for (auto node : sorted) {
			std::vector<Node*> result = revGraph.dfs(node->GetNumber());

			if (!result.empty()) {
				SCC.push_back(result);
			}
		}

		return SCC;
	}

	uint64_t findCount() {
		std::vector<std::vector<Node*>> SCC = findSCC();
		if (SCC.size() == 1 and SCC[0].size() != 1) {
			return 0;
		}
		uint64_t countIn = 0;
		uint64_t countOut = 0;

		for (auto v : SCC) {
			std::sort(v.begin(), v.end(), [](Node* first, Node* second) {
				if (first->GetNumber() > second->GetNumber()) {
					return false;
				}
				return true;
				});
			bool notIn = true;
			bool notOut = true;
			for (auto node : v) {
				std::vector<Node*> In = node->GetNodesIn();
				std::vector<Node*> Out = node->GetNodesOut();
				std::vector<Node*> interIn = intersection(v, In);
				std::vector<Node*> interOut = intersection(v, Out);

				if (In.size() - interIn.size() != 0) {
					notIn = false;
				};
				if (Out.size() - interOut.size() != 0) {
					notOut = false;
				};
			}

			if (notIn) {
				countIn++;
			}
			if (notOut) {
				countOut++;
			}
		}

		return std::max(countIn, countOut);
	}
};

int main() {
	uint64_t nodes, links;
	std::cin >> nodes >> links;

	Graph street(nodes);

	for (auto i = 0; i < links; ++i) {
		uint64_t node1, node2;
		std::cin >> node1 >> node2;

		street.AddLink(node1 - 1, node2 - 1, 1);
	}

	if (links == 1 and nodes == 1) {
		std::cout << 0;
		return 0;
	}

	std::cout << street.findCount();
}