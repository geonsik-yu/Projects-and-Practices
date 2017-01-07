"""
1. Title: Profit Maximization Problem for Production Planning in Automobile Industry

2. Case Description:
Maximize profit of the automobile company based on given spreadsheet informations.

<Spreadsheet 1>
: Expected demands and prices for each (vehicle type, region) pair.
<Spreadsheet 2> 
: Producible models, JPH, available regular working time and overtime and corresponding unit costs for each process line.
<Notes>
- Each vehicle model has 3 production processes(body, painting, assembly).
- Unit cost of each (vehicle, process) pair includes material, labor, and other incidental categories.
- Unit costs of vehicle body and painting is not considered in the spreadsheet (= 0).
- Assume there is no finished goods & WIP inventories. 
- Production system failure or breakdown is not considered.
- Shortage cost is not considered.
"""

import cplex
import xlrd

def read_input(excel_input):
	workbook = xlrd.open_workbook(excel_input)
	production_information = workbook.sheet_by_index(0)
	market_information = workbook.sheet_by_index(1)

	demands = []
	var_names = []
	obj_coeffs = []
	process_info = {}
	model_names = {}
	unit_costs = {}

	## read excel sheet for market demand/price informations
	for row_idx in range(1, market_information.nrows): 
		row = market_information.row_values(row_idx) 
		if row[0] not in model_names.keys():
			model_names[row[0]] = 1
		else:
			model_names[row[0]] += 1
		var_names.extend( (row[0]+repr(int(row[1]))+"_RT", row[0]+repr(int(row[1]))+"_OT") )
		demands.append( row[3] )
		obj_coeffs.extend( (row[4], row[4]) ) ## unit contribution margin for RT/OT initialized by price

	## read excel sheet for production informations
	for name in model_names.keys():
		unit_costs[name] = [0, 0] ## [R.T Cost, O.T Cost]

	for row_idx in range(1, production_information.nrows): 
		row = production_information.row_values(row_idx) 
		for name in row[1].split(","):
			unit_costs[name][0] = unit_costs[name][0] + row[5]
			unit_costs[name][0] = unit_costs[name][0] + row[6]
		process_info[row[0]+"_RT"] = [ "_RT", row[1], row[2], row[3] ]
		process_info[row[0]+"_OT"] = [ "_OT", row[1], row[2], row[4] ]
	for name in var_names:
		## calculate unit contribution margin by substracting cost claimed by each process
		obj_coeffs[ var_names.index(name) ] -= unit_costs[name[0]][0 if name[-2:] == "RT" else 1]
	return [ demands, model_names, var_names, obj_coeffs, process_info ]

def print_output( output ):	
	var_names, const_names, obj_coeffs, problem = output 
	numrows = problem.linear_constraints.get_num()
	numcols = problem.variables.get_num()
	solution_value = problem.solution.get_values()
	shadow_price = problem.solution.get_dual_values()

	print "Solution status = "+ repr(problem.solution.get_status())+ ": " +repr(problem.solution.status[problem.solution.get_status()])
	print "Solution value  = "+ repr(problem.solution.get_objective_value())
	for i in range(numcols):
	    print "Variable " + var_names[i] + ": Coeff = " + repr(obj_coeffs[i]) + "   : Value = " + repr(solution_value[i])
	for i in range(numrows):
	    print "Constraint " + const_names[i] + ": Shadow Price = " + repr(shadow_price[i])


def production_planning( input_data ):
	demands = input_data[0]
	model_names = input_data[1]
	var_names = input_data[2]
	const_names = []
	obj_coeffs = input_data[3]
	process_info = input_data[4]

	lin_exprs = []
	righthand_sides = []
	const_senses = []

	## Prepare "Nonnegativity Constraint" for each variable
	lowerbounds = [0.0]*len(var_names) 

	## Prepare "Expected Demand Constraint" for each (vehicle type, region) pair
	for idx in range(0, len(demands)):
		indices = [ 2*idx, 2*idx+1 ]
		lin_exprs.append( cplex.SparsePair(ind=indices, val=[1.0, 1.0]) )
		righthand_sides.append( demands[idx] )
		const_senses.append( 'L' )
		const_names.append(var_names[2*idx][:-3])
	## Prepare "Available Time Constraints" for each (processes type, RT/OT) pair
	for process in process_info.keys():
		info = process_info[process]	
		temp_vars = []	
		vehecle_types = info[1].split(",")
		for vehicle in vehecle_types:
			for idx in range(0, model_names[vehicle]):
				temp_vars.append(var_names.index(vehicle+repr(int(idx))+info[0]))
		lin_exprs.append( cplex.SparsePair(ind=temp_vars, val=[1.0]*len(temp_vars) ) )
		righthand_sides.append( info[2]*info[3] )
		const_senses.append( 'L' )
		const_names.append(process)

	## CPLEX
	problem = cplex.Cplex()
	problem.objective.set_sense( problem.objective.sense.maximize )
	problem.variables.add( obj=obj_coeffs, lb=lowerbounds, names=var_names )
	problem.linear_constraints.add(lin_expr = lin_exprs, senses = const_senses, rhs = righthand_sides, names = const_names )
	problem.solve()

	return [var_names, const_names, obj_coeffs, problem]

def main(argv=None):
	read_info = read_input("./ProductionPlanning1.xlsx")
	output = production_planning( read_info )
	print_output( output )
if __name__ == "__main__":
    main()

