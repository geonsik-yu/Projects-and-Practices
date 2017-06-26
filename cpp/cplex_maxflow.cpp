// -------------------------------------------------------------- -*- C++ -*-
// File: cplex_maxflow.cpp
// --------------------------------------------------------------------------

/* ------------------------------------------------------------
< Descriptions >

1. Goal
- Determine the maximum flow value of a given flow network with linear programming concepts and CPLEX/ILOG Concert Technology.

2. Notes
- This program reads the input from "input.txt" in the current working directory,
- The 1st line of the input contains the number of vertices n (n <= 1,000) and the number of arcs m, separated by a space.
- Each vertex is numbered from 0 to n-1.
- Each of the following m lines of the input contains (the numeric IDs of) the tail and head of the arc and its capacity, separated by spaces.
- Vertex 0 is the flow source; Vertex n-1 is the sink.
- Assume that influx into the source node and outflux from the sink node are allowed as an input.
- The console output shows the value of a maximum flow using "%.2f\n" for formatting.
- Wrong input not handled.
- Meaningless constraints(e.g. nodes without influxes or outfluxes) not handled.
------------------------------------------------------------ */


#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>

using namespace std;
ILOSTLBEGIN

IloModel read_lp(int& n, int& m, IloEnv env){
	ifstream fin;
	ofstream fout;
	fin.open("./input.txt");
	fout.open("./output.txt");
	if (fin.is_open())
		cout << "input file opened properly\n";
	else { cout << "file open failure\n"; exit(1); }

	string read_str;
	string buf;
	getline(fin, read_str);
	stringstream ss(read_str);
	ss >> buf; n = atoi(buf.c_str());
	ss >> buf; m = atoi(buf.c_str());

	IloModel model(env);
	IloObjective obj = IloMaximize(env);
	IloNumVarArray var(env);
	IloRangeArray con(env, n - 2, 0.0, 0.0); // Flow conservation constraints (equality constraints) for each node except for the source and the sink. 

	while (getline(fin, read_str)){
		int index_i, index_j;
		double capacity;
		double obj_coeff = 0.0;

		string buf;
		stringstream ss(read_str);
		vector<string> line_tokens;
		while (ss >> buf)
			line_tokens.push_back(buf);
		index_i = atoi(line_tokens.at(0).c_str());
		index_j = atoi(line_tokens.at(1).c_str());
		capacity = stod(line_tokens.at(2).c_str());

		// Column-wise Coefficient Inserting Format by CPLEX
		if (index_i == 0 && index_j == n - 1)
			var.add(IloNumVar(obj(1.0), 0.0, capacity));
		else if (index_i == 0)
			var.add(IloNumVar(obj(1.0) + con[index_j - 1](-1.0), 0.0, capacity));
		else if (index_j == n - 1)
			var.add(IloNumVar(obj(0.0) + con[index_i - 1](1.0), 0.0, capacity));
		else
			var.add(IloNumVar(obj(0.0) + con[index_i - 1](1.0) + con[index_j - 1](-1.0), 0.0, capacity));
	}
	fin.close();
	model.add(obj);
	model.add(con);
	return model;
};


int main(int, char**)
{
	int n, m;
	IloEnv env;
	try {
		IloModel model = read_lp(n, m, env);
		IloCplex cplex(model);
		cplex.exportModel("HW1_2015321138.lp");
		if (!cplex.solve()) {
			env.error() << "Failed to optimize LP" << endl;
			throw(-1);
		}
		IloNumArray vals(env);
		env.out() << "Solution status = " << fixed << setprecision(2) << cplex.getStatus() << endl;
		env.out() << "Solution value  = " << fixed << setprecision(2) << cplex.getObjValue() << endl;
	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}
	env.end();
	return 0;
}
