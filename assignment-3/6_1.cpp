#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

struct Node;

struct Node {

    int number;
    std::vector<int> absorbed{};
    std::unordered_map<Node*, int> links;

    explicit Node(int id) : number{ id } {}
};

class Graph {
    uint64_t size;
    std::vector<Node*> nodes;

    void absorb(Node* first, Node* second) {

        first->absorbed.push_back(second->number);
        first->absorbed.insert(first->absorbed.end(), second->absorbed.begin(), second->absorbed.end());

        for (auto node : nodes) {

            node->links.erase(second);

            if (node == second or node == first)
                continue;

            node->links[first] += second->links[node];
            first->links[node] += second->links[node];
        }

        for (auto i = 0; i < nodes.size(); ++i)
            if (nodes[i] == second) {
                nodes.erase(nodes.begin() + i);
                break;
            }

        delete second;
    }

    int min_cut_step(std::vector<int>& answer) {

        std::vector<bool> color(nodes.size(), false);

        color.back() = true;

        std::vector<int> weights(color.size(), 0);

        for (auto cur = 1; cur < nodes.size() - 1; ++cur) {

            weights.assign(color.size(), 0);

            for (auto i = 0; i < color.size(); ++i) {

                if (not color[i])
                    continue;

                for (auto j = 0; j < color.size(); ++j) {

                    if (color[j])
                        continue;

                    weights[j] += nodes[i]->links[nodes[j]];
                }
            }

            int max_weight{ 0 }, id{ 0 };

            for (auto i = 0; i < weights.size(); ++i) {
                if (weights[i] > max_weight) {
                    max_weight = weights[i];

                    id = i;
                }
            }

            color[id] = true;
        }

        int first = 0;

        for (auto i = 0; i < color.size(); ++i)
            if (not color[i]) {
                first = i;
                break;
            }

        int second = 0;

        for (auto i = 0; i < weights.size(); ++i) {
            if (weights[i] and i != first) {
                second = i;
                break;
            }
        }

        int val = 0;

        for (auto i = 0; i < color.size(); ++i) {

            if (i == first)
                continue;

            val += nodes[first]->links[nodes[i]];
        }

        answer.assign(nodes[first]->absorbed.begin(), nodes[first]->absorbed.end());
        answer.push_back(nodes[first]->number);

        absorb(nodes[first], nodes[second]);

        return val;
    }

public:
    Graph(uint64_t size) : size(size) {
        for (auto i = 0; i < size; ++i) {
            nodes.push_back(new Node(i));
        }
    }

    void AddLink(int64_t i, std::string s) {
        for (auto j = i + 1; j < size; ++j) {

            if (s[j] == '0') {

                nodes[i]->links[nodes[j]] = 0;
                nodes[j]->links[nodes[i]] = 0;

                continue;
            }

            nodes[i]->links[nodes[j]] = 1;
            nodes[j]->links[nodes[i]] = 1;
        }
    }

    int min_cut(std::vector<int>& side1) {

        int _min_cut = nodes.size();

        while (nodes.size() != 1) {

            std::vector<int> cur;

            auto tmp = min_cut_step(cur);
            if (_min_cut > tmp) {

                _min_cut = tmp;

                std::swap(side1, cur);
            }
        }

        return _min_cut;
    }
};

int main() {
    int n;
    std::string s;
    std::cin >> n;

    Graph graph(n);

    for (auto i = 0; i < n; ++i) {
        std::cin >> s;
        graph.AddLink(i, s);
    }

    std::vector<int> left;

    int res = graph.min_cut(left);

    std::vector<bool> color(n, false);

    for (auto i : left) {
        std::cout << i + 1 << ' ';
        color[i] = true;
    }

    std::cout << '\n';

    for (auto i = 0; i < n; ++i) {

        if (not color[i])
            std::cout << i + 1 << ' ';
    }

    return 0;
}