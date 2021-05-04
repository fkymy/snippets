#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>

using std::cout;   using std::cerr;
using std::cin;    using std::cin;
using std::endl;   using std::string;
using std::vector; using std::istringstream;

const double kPi = 3.14159265358979323;

// Graph Settings
const double kRepel = 0.001;
const double kAttract = 0.001;
const double numSeconds = 5;
const size_t numNodes = 3;
const string edgeString = "0 1 0 2 1 2";
const vector<vector<size_t> >edgeData { {0, 1}, {0, 2}, {1, 2} };

struct Node {
	double x, y;
};

struct Edge {
	std::size_t start, end;
};

struct SimpleGraph {
	vector<Node> nodes;
	vector<Edge> edges;
};

std::pair<double, double> assignInitialPosition(size_t k, size_t n) {
	double x = cos((2 * kPi * k) / n);
	double y = sin((2 * kPi * k) / n);
	return std::make_pair(x, y);
}

void computeForce(SimpleGraph &graph) {
	vector<double> deltaX(graph.nodes.size());
	vector<double> deltaY(graph.nodes.size());

	for (size_t i = 0; i < graph.nodes.size() - 1; ++i) {
		for (size_t j = i + 1; j < graph.nodes.size(); ++j) {
			Node n1 = graph.nodes[i];
			Node n2 = graph.nodes[j];
			double dist = sqrt((n2.x - n1.x) * (n2.x - n1.x)
					+ (n2.y - n1.y) * (n2.y - n1.y));
			double fRepel = kRepel / dist;
			double theta = atan2((n2.y - n1.y), (n2.x - n1.x));
			deltaX[i] -= fRepel * cos(theta);
			deltaY[i] -= fRepel * sin(theta);
			deltaX[j] += fRepel * cos(theta);
			deltaY[j] += fRepel * sin(theta);
		}
	}

	for (size_t i = 0; i < graph.edges.size(); ++i) {
		size_t startIndex = graph.edges[i].start;
		size_t endIndex = graph.edges[i].end;
		Node n1 = graph.nodes[startIndex];
		Node n2 = graph.nodes[endIndex];
		double fAttract = kAttract * ((n2.x - n1.x) * (n2.x - n1.x)
				+ (n2.y - n1.y) * (n2.y - n1.y));
		double theta = atan2((n2.y - n1.y), (n2.x - n1.x));
		deltaX[startIndex] += fAttract * cos(theta);
		deltaY[startIndex] += fAttract * sin(theta);
		deltaX[endIndex] -= fAttract * cos(theta);
		deltaY[endIndex] -= fAttract * sin(theta);
	}
	for (size_t i = 0; i < graph.nodes.size(); ++i) {
		graph.nodes[i].x += deltaX[i];
		graph.nodes[i].y += deltaY[i];
	}
}

void iterate(SimpleGraph &graph, double numSeconds) {
	time_t startTime;
	double elapsedTime;

	startTime = time(NULL);
	elapsedTime = difftime(time(NULL), startTime);
	while (elapsedTime < numSeconds) {
		computeForce(graph);
		elapsedTime = difftime(time(NULL), startTime);
	}
	cout << "Finished iterating." << endl;
}

void drawGraph(SimpleGraph &graph) {
	for (size_t i = 0; i < graph.nodes.size(); ++i) {
		cout << "node " << i << ": x " << graph.nodes[i].x
			<< ", y " << graph.nodes[i].y << endl;
	}
}

int main() {
	SimpleGraph graph;

	for (size_t i = 0; i < numNodes; ++i) {
		Node node;
		std::tie(node.x, node.y) = assignInitialPosition(i, numNodes);
		graph.nodes.push_back(node);
	}

	istringstream iss(edgeString);
	while (true) {
		size_t start, end;
		iss >> start >> end;
		if (iss.fail()) break;

		Edge edge;
		edge.start = start;
		edge.end = end;
		graph.edges.push_back(edge);
	}

	iterate(graph, numSeconds);
	drawGraph(graph);
	return 0;
}
