#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>

enum class ColorN {
	white, grey, black
};

enum class ColorL {
	green, blue, red, black
};

class Node;

struct Link {
	int64_t val;
	ColorL color;
	Node* node1;
	Node* node2;

	Link(int64_t val, Node* node1, Node* node2) : val(val), color(ColorL::black), node1(node1), node2(node2) {}
};

class Node {
	uint64_t number;
	ColorN color;
	std::vector<Link*> links;

public:
	Node(uint64_t number) : number(number), color(ColorN::white) {}

	void AddLink(Link* link) {
		links.push_back(link);
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

	const std::vector<Link*>& GetLinks() {
		return links;
	}
};

class Graph {
	std::vector<Node*> nodes;
	std::vector<Link*> links;
	uint64_t size;

	Graph(std::vector<Node*> nodes) : nodes(nodes) {}

	std::vector<Node*> SearchCycle() {
		std::vector<Node*> Cycle;
		std::stack<std::pair<Node*, uint64_t>> trail;

		trail.push({ nodes[0], 0 });
		nodes[0]->ChangeColor(ColorN::grey);

		while (!trail.empty()) {
			auto cur = trail.top();

			std::vector<Link*> children = cur.first->GetLinks();

			for (auto i = cur.second; i < children.size(); ++i) {
				Node* node = children[i]->node1 != cur.first ? children[i]->node1 : children[i]->node2;
				if (node->GetColor() == ColorN::grey and children[i]->color != ColorL::green) {
					while (trail.top().first != node) {
						Cycle.push_back(trail.top().first);
						trail.pop();
					}

					Cycle.push_back(node);

					return Cycle;
				}
				else if (node->GetColor() == ColorN::white) {
					children[i]->color = ColorL::green;
					trail.top().second = i;
					trail.push({ node, 0 });
					node->ChangeColor(ColorN::grey);
					break;
				}
			}

			if (trail.top() == cur) {
				cur.first->ChangeColor(ColorN::black);
				trail.pop();
			}
		}

		return Cycle;
	}

	void clear() {
		for (auto node : nodes) {
			node->ChangeColor(ColorN::white);
		}
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
			std::vector<Link*> children = deep.top().first->GetLinks();
			std::pair<Node*, uint64_t> cur = deep.top();

			for (auto i = cur.second; i < children.size(); ++i) {
				Node* node = children[i]->node1 != cur.first ? children[i]->node1 : children[i]->node2;
				if (node->GetColor() == ColorN::white) {
					deep.top().second = i;
					deep.push({ node, 0 });
					node->ChangeColor(ColorN::grey);
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

	std::vector<std::vector<Node*>>  findSCC() {
		std::vector<std::vector<Node*>> SCC;

		for (auto node : nodes) {
			std::vector<Node*> result = dfs(node->GetNumber());

			if (!result.empty()) {
				SCC.push_back(result);
			}
		}
		clear();
		return SCC;
	}

	//std::vector<std::vector<Node*>> Segments(const std::vector<Node*>& cycle) {
	//	std::vector<std::vector<Node*>> segments;

	//	for (auto i = 0; i < cycle.size(); ++i) {
	//		Node* cur = cycle[i];
	//		Node* left = cycle[i != 0 ? i - 1 : cycle.size() - 1];
	//		Node* right = cycle[i != cycle.size() - 1 ? i + 1 : 0];
	//		std::vector<Link*> neighbours = cur->GetLinks();

	//		for (auto link : neighbours) {
	//			if (link->color == ColorL::black) {
	//				link->color = ColorL::green;
	//				Node* node = link->node1 != cur ? link->node1 : link->node2;

	//				if (node != left and node != right) {
	//					auto result = std::find(cycle.begin(), cycle.end(), node);

	//					if (result != cycle.end()) {
	//						segments.push_back({ cur, node });
	//					}
	//					else {
	//						std::vector<std::vector<Node*>> notCycle = { { cur, node } };
	//						uint64_t count = 0;

	//						while (count != notCycle.size()) {
	//							Node* newCur = notCycle[count][notCycle[count].size() - 1];
	//							std::vector<Link*> notCycleNeighbours = newCur->GetLinks();

	//							for (auto newLink : notCycleNeighbours) {
	//								if (newLink->color == ColorL::black) {
	//									newLink->color = ColorL::green;
	//									Node* newNode = newLink->node1 != newCur ? newLink->node1 : newLink->node2;
	//									auto result = std::find(cycle.begin(), cycle.end(), newNode);
	//									std::vector<Node*> newSegment = notCycle[count];
	//									newSegment.push_back(newNode);

	//									if (result != cycle.end()) {
	//										segments.push_back(newSegment);
	//									}
	//									else {
	//										notCycle.push_back(newSegment);
	//									}
	//								}
	//							}

	//							++count;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}

	//	return segments;
	//}

	//void new_edge(std::vector<std::vector<Node*>>& edges, uint64_t oldedge_index, const std::vector<Node*>& segment) {
	//	std::vector<Node*> newedge1, newedge2;
	//	std::vector<Node*> oldedge = edges[oldedge_index];
	//	bool first = false;

	//	for (auto node : oldedge) {
	//		if (node == segment[0]) {
	//			if (first) {
	//				for (auto i = 0; i < segment.size() - 1; ++i) {
	//					newedge2.push_back(segment[i]);
	//				}
	//			}
	//			else {
	//				for (auto i = 0; i < segment.size() - 1; ++i) {
	//					newedge1.push_back(segment[i]);
	//				}
	//			}

	//			first = !first;
	//		}
	//		else if (node == segment[segment.size() - 1]) {
	//			if (first) {
	//				for (auto i = 0; i < segment.size() - 1; ++i) {
	//					newedge2.push_back(segment[segment.size() - i - 1]);
	//				}
	//			}
	//			else {
	//				for (auto i = 0; i < segment.size() - 1; ++i) {
	//					newedge1.push_back(segment[segment.size() - i - 1]);
	//				}
	//			}

	//			first = !first;
	//		}

	//		if (!first) {
	//			newedge1.push_back(node);
	//		}
	//		else {
	//			newedge2.push_back(node);
	//		}
	//	}

	//	edges[oldedge_index] = newedge1;
	//	edges.push_back(newedge2);
	//}

	//bool _gamma_alghoritm(Graph SCC) {
	//	std::vector<Node*> cycle = SCC.SearchCycle();
	//	std::vector<std::vector<Node*>> edges = { cycle, cycle };
	//	std::vector<std::vector<Node*>> segments = SCC.Segments(cycle);

	//	while (!segments.empty()) {
	//		uint64_t minEdges = edges.size() + 1;
	//		uint64_t minEdgesIndex = 0;
	//		uint64_t minSegmentsIndex = 0;

	//		for (auto i = 0; i < segments.size(); ++i) {
	//			if (segments[i][0] != nullptr) {
	//				uint64_t Edges = 0;
	//				uint64_t minEdgesIndexL = 0;
	//				for (auto j = 0; j < edges.size(); j++) {
	//					auto result1 = std::find(edges[j].begin(), edges[j].end(), segments[i][0]);
	//					auto result2 = std::find(edges[j].begin(), edges[j].end(), segments[i][segments[i].size() - 1]);

	//					if (result1 != edges[j].end() and result2 != edges[j].end()) {
	//						++Edges;
	//						minEdgesIndexL = j;
	//					}
	//				}

	//				if (Edges == 0) {
	//					return false;
	//				}

	//				if (Edges < minEdges) {
	//					minEdges = Edges;
	//					minSegmentsIndex = i;
	//					minEdgesIndex = minEdgesIndexL;
	//				}
	//			}
	//		}

	//		new_edge(edges, minEdgesIndex, segments[minSegmentsIndex]);
	//		segments[minSegmentsIndex][0] = nullptr;
	//	}

	//	return true;
	//}

public:
	Graph(uint64_t size) : size(size) {
		for (uint64_t i = 0; i < size; ++i) {
			nodes.push_back(new Node(i));
		}
	}

	void AddLink(uint64_t in, uint64_t out, int64_t val) {
		if (in != out) {
			Link* NewLink = new Link(val, nodes[in], nodes[out]);

			nodes[in]->AddLink(NewLink);
			nodes[out]->AddLink(NewLink);

			links.push_back(NewLink);
		}
	}

	//bool gamma_alghoritm() {
	//	std::vector<std::vector<Node*>> allSCC = findSCC();

	//	for (auto scc : allSCC) {
	//		if (scc.size() > 1) {
	//			Graph SCC = { scc };
	//			SCC.clear();
	//			if (!_gamma_alghoritm(SCC)) {
	//				return false;
	//			}
	//		}
	//	}

	//	return true;
	//}



};

int main() {
	//uint64_t nodes, links;
	//std::cin >> nodes >> links;
	//
	//Graph task(nodes);

	//for (auto i = 0; i < links; ++i) {
	//	uint64_t node1, node2;
	//	std::cin >> node1 >> node2;

	//	task.AddLink(node1, node2, 1);
	//}

	//std::cout << task.gamma_alghoritm();

}