/*
 Title: Cplex, Minimum Cut from Maximum Flow(Cplex solution)'s Residual Graph.
 Description: This c++ source code finds minimum cut and its value of the given directed flow network.
 Note:
 	- Develop with IBM ILOG CPLEX/Concert in OS X.
	- First, we find maximum flow solution by using linear programming and cplex
		and then find correcponding minimum cut from the solution's resulting residual graph.
 		>> Given a graph G=(V,E) and flow f_e for each edge e, corresponding residual graph G'=(V,E') is as follows:
			- for every edge e in E with capacity c_e
				(1) A forward edge e' is in E' with capacity (c_e - f_e) if (c_e - f_e) is not zero.
				(2) A backward(opposite direction) edge e'' is in E' with capacity f_e if f_e is not zero.
		>> Given a residual graph G'=(V,E'), 
 			- minimum cut C*:= {<u,v>|<u,v> in E', where u is reachable from the source node and v in not reachable from the source}

	- The 1st line of the input contains the number of vertices n (n <= 1,000) and the number of arcs m, separated by a space.
 	- Each vertex is numbered from 0 to n-1.
 	- Each of the following m lines of the input contains (the numeric IDs of) the tail and head of the arc and its capacity, separated by spaces.
 	- Vertex 0 is the flow source; Vertex n-1 is the sink.
 	- Assume that influx into the source node and outflux from the sink node are not allowed as an input.
 	- Wrong input not handled.
 	- Meaningless constraints(e.g. nodes without influxes or outfluxes) not manually detected or removed.
*/

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Structures used for graph.
struct AdjListNode{
	int dest;
	int capacity;
	int residual;
	int index;
	struct AdjListNode* next = NULL;
	AdjListNode(int destArg, int costArg, struct AdjListNode* node, int indArg){
		dest = destArg;
		capacity = costArg;
		residual = 0;
		next = node;
		index = indArg;
	}
};
struct AdjList{
	struct AdjListNode* head = NULL;
};

// Static variables.
static int n;
static int m;
static int maxFlowValue;
static int minCutValue;
static struct AdjList* residualGraph;
static int* reachables;

ILOSTLBEGIN
bool minCutProblem(string inputDirectory);
void findMaxFlow(string inputDirectory);
void findReachables();
void findMinCut();

int main(){
	for(int i = 0; i < 100; i++){
		cout << "number::" << i << endl;
		maxFlowValue = 0;
		minCutValue = 0;
		minCutProblem("./DWFN/net" + to_string(i+1) + ".txt");
	}
	return 0;
};

bool minCutProblem(string inputDirectory){
	try { findMaxFlow(inputDirectory);}
	catch (IloException& e) { cerr << "Concert exception caught: " << e << endl;}
	catch (...) { cerr << "Unknown exception caught" << endl; }

	// Initialize array.
	// 0: not reachable, 1: reachable from source, 2: reachable from sink
	reachables = new int[n];
	for(int i = 0; i < n; i++)
		reachables[i] = 0;
	
	findReachables();
	findMinCut();
	
	cout << "Maximul Flow Value: " << maxFlowValue << endl;
	cout << "Minimum Cut Value: " << minCutValue << endl;
	cout << "-----------------------------------------\n";
	delete[] reachables;
	
	if( maxFlowValue == minCutValue)
		return true;
	else
		return false;
};

void findMaxFlow(string inputDirectory){
	ifstream fin;
	ofstream fout;
	fin.open(inputDirectory);
	fout.open("result_"+inputDirectory);
	if (!fin.is_open()){ cout << "file open failure\n"; exit(1); }
	
	string read_str;
	string buf;
	getline(fin, read_str);
	stringstream ss(read_str);
	ss >> buf; n = atoi(buf.c_str());
	ss >> buf; m = atoi(buf.c_str());
	
	residualGraph = new AdjList[n];
	IloEnv env;
	IloModel model(env);
	IloObjective obj = IloMaximize(env);
	IloNumVarArray var(env);
	IloRangeArray con(env, n - 2, 0.0, 0.0);
	// Flow conservation constraints (equality constraints) for each node except for the source and the sink.
	
	int var_index = 0;
	while (getline(fin, read_str)){
		int index_i, index_j;
		int capacity;
		
		string buf;
		stringstream ss(read_str);
		vector<string> line_tokens;
		while (ss >> buf)
		line_tokens.push_back(buf);
		index_i = atoi(line_tokens.at(0).c_str());
		index_j = atoi(line_tokens.at(1).c_str());
		capacity = atoi(line_tokens.at(2).c_str());
		
		// Column-wise coefficient inserting.
		//if (index_i == n-1)
		if (index_i == 0 && index_j == n - 1)
			var.add(IloNumVar(obj(1.0), 0.0, capacity));
		else if (index_i == 0)
			var.add(IloNumVar(obj(1.0) + con[index_j - 1](-1.0), 0.0, capacity));
		else if (index_j == n - 1)
			var.add(IloNumVar(obj(0.0) + con[index_i - 1](1.0), 0.0, capacity));
		else
			var.add(IloNumVar(obj(0.0) + con[index_i - 1](1.0) + con[index_j - 1](-1.0), 0.0, capacity));
		
		// Build residual graph (1)
		// - initialize with full capacities.
		struct AdjListNode* temp = residualGraph[index_i].head;
		residualGraph[index_i].head = new AdjListNode(index_j, capacity, temp, var_index);
		var_index++;
	}
	fin.close();
	model.add(obj);
	model.add(con);

	IloCplex cplex(model);
	cplex.setOut(env.getNullStream());
 
	cplex.exportModel("MinCut_from_MaxFow.lp");
	if (!cplex.solve()) {
		env.error() << "Failed to optimize LP" << endl;
		throw(-1);
	}
	
	IloNumArray solutions(env);
	cplex.getValues(solutions, var);
	
	
	// Build residual graph (2)
	// - add forward/backward edges.
	for (int i = 0; i < n; i++){
		struct AdjListNode* pCrawl = residualGraph[i].head;
		while (pCrawl != NULL){
			if(pCrawl -> index != -1){
				pCrawl -> residual = pCrawl -> capacity - solutions[pCrawl -> index];
				if(solutions[pCrawl -> index] != 0){
					struct AdjListNode* backward = residualGraph[pCrawl->dest].head;
					residualGraph[pCrawl->dest].head = new AdjListNode(i, -1, backward, -1);
					residualGraph[pCrawl->dest].head -> residual = solutions[pCrawl -> index];
				}
			}
			pCrawl = pCrawl->next;
		}
	}
	
	maxFlowValue = cplex.getObjValue();
	//env.out() << "Solution status = " << fixed << setprecision(2) << cplex.getStatus() << endl;
	//env.out() << "Solution value  = " << fixed << setprecision(2) << cplex.getObjValue() << endl;
	
	env.end();
};

void findReachables(){
	vector<int> candidates;
	reachables[0] = 1;
	struct AdjListNode* pCrawl;
	
	// Find reachable nodes from the source nodes (Verify source-side cut).
	candidates.push_back(0);
	while(candidates.size() != 0){
		pCrawl = residualGraph[candidates.back()].head;
		candidates.pop_back();
		while (pCrawl != NULL){
			if( pCrawl -> residual > 0 && reachables[pCrawl->dest] == 0){
				reachables[pCrawl->dest] = 1;
				candidates.push_back( pCrawl->dest );
			}
			pCrawl = pCrawl->next;
		}
	}
};

void findMinCut(){
	for(int i = 0; i < n; i++){
		struct AdjListNode* pCrawl = residualGraph[i].head;
		while (pCrawl != NULL){
			if( reachables[i] == 1 && reachables[pCrawl->dest] != 1 ){
				cout << "(" << i << "-" << pCrawl->dest << ") with capacity = " << pCrawl -> capacity << endl;
				minCutValue += pCrawl -> capacity;
			}
			pCrawl = pCrawl->next;
		}
	}
};
