#include <iostream>

struct Node {
	std::string value;
	uint64_t size;
	int64_t priority;
	Node* left;
	Node* right;

	Node(const std::string& value) : value(value), priority(rand()), left(nullptr), right(nullptr), size(1){}

	void resize() {
		size = 1;
		if (left != nullptr)
			size += left->size;
		if (right != nullptr)
			size += right->size;
	}
};

class Treap {
	Node* root;

	std::pair<Node*, Node*> split(Node* node, uint64_t position) {
		if (node == nullptr)
			return { nullptr, nullptr };

		uint64_t l = (node->left != nullptr) ? (node->left->size) : 0;

		if (l >= position) {
			std::pair<Node*, Node*> new_trees = split(node->left, position);

			node->left = new_trees.second;
			node->resize();
			return { new_trees.first, node };
		}
		else {
			std::pair<Node*, Node*> new_trees = split(node->right, position - l - 1);

			node->right = new_trees.first;
			node->resize();
			return { node, new_trees.second };
		}
	}

	Node* merge(Node* node1, Node* node2) {
		if (node2 == nullptr)
			return node1;

		if (node1 == nullptr)
			return node2;

		if (node1->priority > node2->priority) {
			node1->right = merge(node1->right, node2);
			node1->resize();
			return node1;
		}
		else {
			node2->left = merge(node1, node2->left);
			node2->resize();
			return node2;
		}
	}

	void clear(Node* node) {
		if (node == nullptr)
			return;

		clear(node->left);
		clear(node->right);

		delete node;
	}

public:
	Treap() : root(nullptr){}
	~Treap() {
		clear(root);
		root = nullptr;
	};

	void insert(uint64_t pos, const std::string& value) {
		Node* cur = new Node(value);

		std::pair<Node*, Node*> new_trees = split(root, pos);
		root = merge(new_trees.first, merge(cur, new_trees.second));
	}

	void remove(uint64_t pos1, uint64_t pos2) {
		std::pair<Node*, Node*> new_trees1 = split(root, pos1);
		std::pair<Node*, Node*> new_trees2 = split(new_trees1.second, pos2 - pos1 + 1);

		delete new_trees2.first;

		root = merge(new_trees1.first, new_trees2.second);
	}

	std::string get(uint64_t pos) {
		uint64_t ns;
		Node* node = root;

		while (true) {
			ns = (node->left != nullptr) ? (node->left->size) : 0;
			if (ns == pos)
				break;
			if (pos < ns) {
				node = node->left;
			}
			else {
				pos -= ns + 1;
				node = node->right;
			}
		} 

		return node->value;
	}
};

int main() {
	uint64_t n;
	std::cin >> n;
	Treap stringArray;

	for (auto i = 0; i < n; ++i) {
		char cmd;
		std::cin >> cmd;

		if(cmd == '+'){
			uint64_t pos;
			std::string val;
			std::cin >> pos >> val;

			stringArray.insert(pos, val);
		}
		else if(cmd == '?') {
			uint64_t pos;
			std::cin >> pos;

			std::cout << stringArray.get(pos) << '\n';
		}
		else {
			uint64_t pos1, pos2;
			std::cin >> pos1 >> pos2;

			stringArray.remove(pos1, pos2);
		}
	}
}