#include <iostream>
#include <limits.h>
#include <utility>
#include <vector>
#include <queue>
#include <time.h>
#include <fstream>

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

using Vertex = size_t;


class DirectedWeightedGraph
{
	std::vector<std::vector<std::pair<size_t, Vertex>>> adjacency_list;

public:

	DirectedWeightedGraph(size_t vertices);

	void addEdge(Vertex from, Vertex to, size_t distance);

	std::vector<std::pair<size_t, Vertex>> getAdj(Vertex v);

	size_t size();
};


DirectedWeightedGraph::DirectedWeightedGraph(size_t verticies):
	adjacency_list(verticies) {}


void DirectedWeightedGraph::addEdge(Vertex from, Vertex to, size_t distance)
{
	adjacency_list.at(from).push_back({ distance, to });
}


std::vector<std::pair<size_t, Vertex>> DirectedWeightedGraph::getAdj(Vertex v)
{
	return adjacency_list.at(v);
}


size_t DirectedWeightedGraph::size()
{
	return adjacency_list.size();
}


void readEdges(DirectedWeightedGraph& graph, size_t dist1, size_t dist2)
{
	for (size_t i = 0, size = graph.size(); i < size; ++i)
	{
		graph.addEdge(i, (i + 1) % size, dist1);
		//std::cout << i << "->" << (i + 1) % size << ": " << dist1 << "\n";
		graph.addEdge(i, (i * i + 1) % size, dist2);
		//std::cout << i << "->" << (i * i + 1) % size << ": " << dist2 << "\n";
	}
}


size_t Dijkstra(DirectedWeightedGraph& graph, Vertex start, Vertex finish)
{
	if (start == finish) return 0;

	std::priority_queue<std::pair<size_t, Vertex>,
		std::vector<std::pair<size_t, Vertex>>,
		std::greater<std::pair<size_t, Vertex>>> pq;

	std::vector<size_t> distances(graph.size(), INT_MAX);

	distances[start] = 0;

	pq.push({ 0, start });

	while (!pq.empty())
	{
		Vertex curr_vertex = pq.top().second;
		size_t curr_dist = pq.top().first;

		pq.pop();

		std::vector<std::pair<size_t, Vertex>> adj = graph.getAdj(curr_vertex);

		for (size_t i = 0, size = adj.size(); i < size; ++i)
		{
			if (adj[i].first + curr_dist < distances[adj[i].second])
			{
				distances[adj[i].second] = adj[i].first + curr_dist;

				pq.push({ distances[adj[i].second], adj[i].second });
			}
		}
	}

	return distances[finish];
}


int main()
{
	std::ofstream outf("test.txt");
	srand(time(0));
	for (auto i = 0; i < 1000; ++i) {
		size_t dist1 = getRandomNumber(1,100), dist2 = getRandomNumber(1,100);
		Vertex verticies = 100, start = getRandomNumber(1, 99), finish = getRandomNumber(1, 99);

		DirectedWeightedGraph graph(verticies);

		readEdges(graph, dist1, dist2);

		outf << dist1 << ' ' << dist2 << ' ' << verticies << ' ' << start << ' ' << finish << std::endl;
		outf << Dijkstra(graph, start, finish) << std::endl;
	}
}
