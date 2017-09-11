//
//  matroidIntersection.cpp
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

struct AdjListNode{
	int dest;
	int cost_1;	// cost_1 of the arc(initially given cost)
	int cost_2; // cost_2 of the arc(initially 0)
	bool intersection = false;
	// if it is in current [set I] then "true", otherwise "false"
	int exchangeIndex;
	// if it represents an edge of the given (undirected) graph
	// then this variable retores corresponding node index in the exchange graph.
	struct AdjListNode* next = NULL;
	AdjListNode(int destArg, int cost_1Arg, int cost_2Arg, struct AdjListNode* node){
		dest = destArg;
		cost_1 = cost_1Arg;
		cost_2 = cost_2Arg;
		next = node;
	}
	AdjListNode(int destArg, int cost_1Arg, int cost_2Arg, struct AdjListNode* node, int exch){
		dest = destArg;
		cost_1 = cost_1Arg;
		cost_2 = cost_2Arg;
		next = node;
		exchangeIndex = exch;
	}
};
struct AdjList{
	struct AdjListNode* head = NULL;
};

struct PathResult{
	int previous, distance;
};


static int n, m, l;
static int maxWeight = 0;
static int* bounds;			   // A dynamic array for degree bounds.
static AdjList* givenGraph;    // A head pointer for the given graph.
static AdjList* exchangeGraph; // restores a current exchange graph corresponding to the given graph and current [set I].
/*
 in the given G=(V,E)
 - n: number of nodes
 - m: number of edges
 - l: number of nodes in U
 */

void read_file(string filepath);
void addArcIndependenceGraphicMatroid();
void addArcIndependencePartitionMatroid();
void buildExchangeGraph();
PathResult* shortestPath(struct AdjList* list, int size, int src, int dest);
vector<int> trackPath(struct PathResult* path, int size, int src, int dest);
vector<int> trackReachables(struct PathResult* path, int size);
int printIntersectionGraph(struct AdjList* list, int size);
void printEntireGraph(struct AdjList* list, int size);
void deleteGraph(struct AdjList* list, int size);
int matroidIntersection();

int main(int argc, const char * argv[]) {
	read_file("./input.txt");
	
	int k = matroidIntersection();
	
	cout << "Final set I is as following:" << endl;
	int value = printIntersectionGraph(givenGraph, n);
	cout << "Final size of set I is " << k << endl;
	
	ofstream fout;
	fout.open("./output.txt");
	if (k < n-1){
		// current result is not a spanning tree.
		fout << "infeasible" << endl;
	}else{
		// current result is a spanning tree.
		fout << value << endl;
		for (int v = 0; v < n; v++){
			struct AdjListNode* pCrawl = givenGraph[v].head;
			while (pCrawl != NULL){
				if( (pCrawl -> dest > v) && (pCrawl -> intersection == true) ){
					fout << v << " " << pCrawl -> dest << endl;
				}
				pCrawl = pCrawl->next;
			}
		}
	}
	fout.close();
	
	delete bounds;
	deleteGraph(givenGraph, n);
	deleteGraph(exchangeGraph, m+2);
	return 0;
}



void read_file(string filepath);
int matroidIntersection();
PathResult* shortestPath(struct AdjList* list, int size, int src, int dest);
vector<int> trackPath(struct PathResult* path, int size, int src, int dest);
vector<int> trackReachables(struct PathResult* path, int size);
int printIntersectionGraph(struct AdjList* list, int size);
void printEntireGraph(struct AdjList* list, int size);
void deleteGraph(struct AdjList* list, int size);

void read_file(string filepath){
	ifstream fin;
	fin.open(filepath);
	
	string read_str;
	string buffer;
	vector<string> line_tokens;
	
	getline(fin, read_str);
	stringstream ss(read_str);
	
	ss >> buffer; n = atoi(buffer.c_str());
	ss >> buffer; m = atoi(buffer.c_str());
	ss.str(""); ss.clear();
	
	givenGraph = new AdjList[n];
	exchangeGraph = new AdjList[m+2];
	/* Exchange Graph
	 < Phantom Nodes >
	 s := m-th node
	 t := (m+1)th node
	 */
	for(int i = 0; i < m; i++){
		int index_i, index_j, weight;
		
		getline(fin, read_str);
		ss.str(read_str);
		for(int j = 0; j < 3; j++){
			ss >> buffer;
			line_tokens.push_back(buffer);
		}
		ss.str(""); ss.clear();
		index_i = atoi(line_tokens.at(0).c_str());
		index_j = atoi(line_tokens.at(1).c_str());
		weight = atoi(line_tokens.at(2).c_str());
		maxWeight = max(weight, maxWeight);
		
		struct AdjListNode* temp = NULL;
		temp = givenGraph[index_i].head;
		givenGraph[index_i].head = new AdjListNode(index_j, -weight, 0, temp, i);
		temp = givenGraph[index_j].head;
		givenGraph[index_j].head = new AdjListNode(index_i, -weight, 0, temp, i);
		line_tokens.clear();
	}
	for (int v = 0; v < n; v++){
		struct AdjListNode* pCrawl = givenGraph[v].head;
		while (pCrawl != NULL){
			pCrawl -> cost_1 += maxWeight;
			pCrawl = pCrawl->next;
		}
	}
	
	//printEntireGraph(givenGraph, n);
	getline(fin, read_str);
	ss.str(read_str);
	ss >> buffer; l = atoi(buffer.c_str());
	ss.str(""); ss.clear();
	
	bounds = new int[n];
	for(int i = 0; i < n; i++) bounds[i] = m+1;
	for(int i = 0; i < l; i++){
		getline(fin, read_str);
		ss.str(read_str);
		int boundIndex;
		ss >> boundIndex;
		ss >> bounds[boundIndex];
		ss.str(""); ss.clear();
	}
	fin.close();
}

void addArcIndependenceGraphicMatroid(){
	int m1 = 0;
	for (int v = 0; v < n; v++){
		struct AdjListNode* pCrawl = givenGraph[v].head;
		
		while (pCrawl != NULL){
			if( (pCrawl -> dest > v) && (pCrawl -> intersection == false) ){
				// for each edge not in the current [set I].
				int cost_x = pCrawl -> cost_1;
				int index_x = pCrawl -> exchangeIndex;
				
				vector<int> path = trackPath( shortestPath(givenGraph, n, v, pCrawl->dest), n, v, pCrawl->dest);
				if (!path.empty()){
					// Case 1: <y, x>: (I-y+x) is independent in the graphic matroid
					int index_i, index_j;
					index_j = path.front(); path.erase(path.begin());
					while(!path.empty()){
						index_i = index_j;
						index_j = path.front(); path.erase(path.begin());
						struct AdjListNode* pathCrawl = givenGraph[index_i].head;
						while (pathCrawl -> dest != index_j) pathCrawl = pathCrawl->next;
						int cost_y = pathCrawl -> cost_1;
						int index_y = pathCrawl -> exchangeIndex;
						
						struct AdjListNode* temp = NULL;
						temp = exchangeGraph[index_y].head;
						exchangeGraph[index_y].head = new AdjListNode(index_x, cost_y - cost_x, 0, temp);
						exchangeGraph[index_y].head->intersection = true;
					}
				}
				else{
					// Case 3: <s, x>: (I+x) is independent in the graphic matroid
					struct AdjListNode* temp = NULL;
					temp = exchangeGraph[m].head;
					exchangeGraph[m].head = new AdjListNode(index_x, - cost_x, 0, temp);
					exchangeGraph[m].head->intersection = true;
					m1 = max(m1, cost_x);
					//cout << index_x<<"hereehere!!" << endl;
				}
			}
			pCrawl = pCrawl->next;
		}
	}
	struct AdjListNode* pCrawl = exchangeGraph[m].head;
	while (pCrawl != NULL){
		pCrawl -> cost_1 += m1;
		pCrawl = pCrawl->next;
	}
}

void addArcIndependencePartitionMatroid(){
	int m2 = 0;
	int* degrees = new int[n]; // degree for each vertex
	for (int v = 0; v < n; v++) degrees[v] = 0; // init
	for (int v = 0; v < n; v++){
		struct AdjListNode* pCrawl = givenGraph[v].head;
		while (pCrawl != NULL){
			if( (pCrawl -> dest > v) && (pCrawl -> intersection == true) ){
				degrees[v]++;
				degrees[pCrawl -> dest]++;
			}
			pCrawl = pCrawl->next;
		}
	}
	
	for (int v = 0; v < n; v++){
		struct AdjListNode* pCrawl = givenGraph[v].head;
		
		while (pCrawl != NULL){
			if( (pCrawl -> dest > v) && (pCrawl -> intersection == false) ){
				// for each edge not in the current intersection(I).
				int cost_x = pCrawl -> cost_2;
				int index_x = pCrawl -> exchangeIndex;
				
				if ( (degrees[v] < bounds[v] ) && ( degrees[pCrawl -> dest] < bounds[pCrawl -> dest] )){
					// Case 4: <x, t>
					struct AdjListNode* temp = NULL;
					temp = exchangeGraph[index_x].head;
					exchangeGraph[index_x].head = new AdjListNode(m+1, - cost_x, 0, temp);
					exchangeGraph[index_x].head->intersection = true;
					m2 = max(m2, cost_x);
				}
				else {
					// Case 2: <x, y>
					int consider = (degrees[v] == bounds[v]) ? v: pCrawl -> dest;
					
					struct AdjListNode* considerCrawl;
					considerCrawl = givenGraph[consider].head;
					while (considerCrawl != NULL) {
						if( considerCrawl -> intersection == true ){
							int cost_y = considerCrawl -> cost_2;
							int index_y = considerCrawl -> exchangeIndex;
							struct AdjListNode* temp = NULL;
							temp = exchangeGraph[index_x].head;
							exchangeGraph[index_x].head = new AdjListNode(index_y, cost_y - cost_x, 0, temp);
							exchangeGraph[index_x].head->intersection = true;
						}
						considerCrawl = considerCrawl->next;
					}
				}
			}
			pCrawl = pCrawl->next;
		}
	}
	for (int v = 0; v < n; v++){
		struct AdjListNode* pCrawl = exchangeGraph[v].head;
		while (pCrawl != NULL){
			if (pCrawl -> dest == m+1)
				pCrawl -> cost_1 += m2;
			pCrawl = pCrawl->next;
		}
	}
	delete[] degrees;
	
}


void buildExchangeGraph(){
	// Arc addition: operations that secure the independence of I in the 1st matroid (graphic matroid).
	addArcIndependenceGraphicMatroid();
	// Arc addition: operations that secure the independence of I in the 2nd matroid (partition matroid).
	addArcIndependencePartitionMatroid();
}


PathResult* shortestPath(struct AdjList* list, int size, int src, int dest){
	// Given Graph, size of it, source node and intended destination.
	// Implementation of Bellman-Ford Algorithm
	// Used (a) to establish R(:= Reachable nodes from s in the given exchange graph).
	//      (b) to track s-t path in the given exchange graph (if exists).
	
	// if size equals to n
	// 	then assume the argument graph is the given graph of our problem, and find shortest path upon edges included in the [set I]
	// if size equals to (m+2)
	// 	then assume the argument graph is the exchange graph, and find shortest path upon edges whose cost is 0
	
	struct PathResult* info = new PathResult[size];
	for (int i = 0; i < size; i++){
		info[i].distance = size + 2;
		info[i].previous = -1;
	}
	info[src].distance = 0;
	info[src].previous = -2;
	
	for (int i = 0; i < size - 1; i++){
		for (int v = 0; v < size; v++){
			struct AdjListNode* pCrawl = list[v].head;
			while (pCrawl != NULL){
				if ( ( size == n ) && ( pCrawl -> intersection == true)
					and ( (info[v].distance + 1) < info[pCrawl -> dest].distance) ){
					info[pCrawl -> dest].distance = info[v].distance + 1;
					info[pCrawl -> dest].previous = v;
				}
				if ( ( size == m+2 ) && ( pCrawl -> cost_1 == 0)
					and ( (info[v].distance + 1) < info[pCrawl -> dest].distance) ){
					info[pCrawl -> dest].distance = info[v].distance + 1;
					info[pCrawl -> dest].previous = v;
				}
				pCrawl = pCrawl->next;
			}
		}
	}
	return info;
}

vector<int> trackPath(struct PathResult* path, int size, int src, int dest){
	vector<int> result;
	if(path[dest].previous != -1){
		int node = dest;
		while( path[node].previous != -2){
			result.push_back(node);
			node = path[node].previous;
		}
		result.push_back(src);
	}
	return result;
}

vector<int> trackReachables(struct PathResult* path, int size){
	vector<int> result;
	for(int i = 0; i < size; i++)
		if( path[i].previous != -1 )
			result.push_back(i);
	return result;
}

int printIntersectionGraph(struct AdjList* list, int size){
	int sum = 0;
	cout << "-------------------------------------------" << endl;
	for (int v = 0; v < size; v++){
		struct AdjListNode* pCrawl = list[v].head;
		cout << "\n Adjacency list of vertex " << v << "\n  head";
		while (pCrawl != NULL){
			if(pCrawl -> intersection == true){
				cout << " -> " << pCrawl -> dest << "("<<pCrawl -> cost_1 << ", " << pCrawl -> cost_2 << ")";
				sum += maxWeight - (pCrawl -> cost_1 + pCrawl -> cost_2);
			}
			pCrawl = pCrawl->next;
		}
		cout << "\n\n";
	}
	cout << "-------------------------------------------" << endl;
	return sum/2;
}
void printEntireGraph(struct AdjList* list, int size){
	cout << "-------------------------------------------" << endl;
	for (int v = 0; v < size; v++){
		struct AdjListNode* pCrawl = list[v].head;
		cout << "\n Adjacency list of vertex " << v << "\n  head";
		while (pCrawl != NULL){
			cout << " -> " << pCrawl -> dest << "("<<pCrawl -> cost_1 << ")";
			pCrawl = pCrawl->next;
		}
		cout << "\n\n";
	}
	cout << "-------------------------------------------" << endl;
}

void deleteGraph(struct AdjList* list, int size){
	for (int v = 0; v < size; v++){
		while(list[v].head != NULL){
			struct AdjListNode* temp = list[v].head -> next;
			delete[] list[v].head;
			list[v].head = temp;
		}
	}
}

int matroidIntersection(){
	int epsilon = 0;
	int k = 0;
	while( true ){
		deleteGraph(exchangeGraph, m+2);
		buildExchangeGraph();
		
		cout << "currently k is " << k/2 << endl;
		PathResult* pathResult = shortestPath(exchangeGraph, m+2, m, m+1);
		
		epsilon = INT_MAX/4;
		if(pathResult[m+1].previous != -1){
			// "if there exists an s-t path" clause
			// Update intersection matroid(given graph).
			// 	I_(k+1) := Symmetric Difference of I_k and (Path\{s,t})
			
			vector<int> path = trackPath(pathResult, m+2, m, m+1);
			k = 0;
			for (int v = 0; v < n; v++){
				struct AdjListNode* pCrawl = givenGraph[v].head;
				while (pCrawl != NULL){
					if( (pCrawl->intersection == true) && !(find(path.begin(), path.end(), (pCrawl->exchangeIndex)) != path.end()) ){
						pCrawl -> intersection = true;
						k++;
					} else if( (pCrawl->intersection == false) && (find(path.begin(), path.end(), (pCrawl->exchangeIndex)) != path.end()) ){
						pCrawl -> intersection = true;
						k++;
					} else {
						pCrawl -> intersection = false;
					}
					pCrawl = pCrawl->next;
				}
			}
			epsilon = 0;
		}
		else{
			// "if there does not exist an s-t path" clause
			epsilon = INT_MAX/4;
			vector<int> reachables = trackReachables(pathResult, m+2); // indices of edges in the given graph
			for (int w = 0; w < m+2; w++){
				struct AdjListNode* pCrawl = exchangeGraph[w].head;
				while (pCrawl != NULL){
					if( (find(reachables.begin(), reachables.end(), w) != reachables.end()) 					  // source reachable
					   and !(find(reachables.begin(), reachables.end(), pCrawl->dest) != reachables.end())){ // destination not reachable
						epsilon = min(epsilon, pCrawl->cost_1);
					}
					pCrawl = pCrawl->next;
				}
			}
			
			if( epsilon < INT_MAX/4 ){
				// "if epsilon is finite" clause
				// Update cost_1 and cost_2 of the given graph.
				for (int v = 0; v < n; v++){
					struct AdjListNode* pCrawl = givenGraph[v].head;
					while (pCrawl != NULL){
						if( find(reachables.begin(), reachables.end(), pCrawl->exchangeIndex) != reachables.end() ){
							// if an edge is reachable in the exchange graph
							pCrawl -> cost_1 -= epsilon;
							pCrawl -> cost_2 += epsilon;
						}
						pCrawl = pCrawl->next;
					}
				}
			}
		}
		if(epsilon >= INT_MAX/4)
			break;
	}
	return k/2;
}
