"""
OR Programming Study #4
< Cplex API 활용 연습: 선형계획법 >
2015년 겨울방학 Python Programming Study
발제자: 유건식
1. 학습 목표: Cplex API 활용법을 익히고, 복수의 LP 문제를 자동화하여 해를 제시하는 프로그램을 구현한다.
2. 설명:
- IBM CPLEX Studio 설치 요령
1. IBM CPLEX Optimization Studio, Community Edition 다운로드 및 설치
- http://www-01.ibm.com/software/websphere/products/optimization/cplex-studio-community-edition/ 
- 변수 1000개, 제약조건 1000개 이하의 선형계획 문제까지 처리 가능한 버전.
- 각자의 운영체제에 맞추어 설치할 것.

2. CPLEX의 Python API 설정(원문)
- CPLEX의 Python API는 IBM ILOG CPLEX Optimization Studio의 일부이다. CPLEX Python API와 연관된 모듈은 yourCPLEXhome/python/PLATFORM 디렉토리(또는 yourCPLEXhome\python\PLATFORM 폴더)에 있다. 여기서, yourCPLEXhome은 IBM ILOG CPLEX Optimization Studio의 일부로 CPLEX가 설치된 위치를 지정하며 PLATFORM은 운영 체제와 컴파일러의 조합을 나타낸다.

- 시스템에 CPLEX-Python 모듈을 설치하려면 yourCplexhome/python/PLATFORM에 있는 setuy.py 스크립트를 사용한다. 기본이 아닌 위치에 CPLEX-Python 모듈을 설치하려면 --home 옵션을 사용하여 설치 디렉토리를 식별하면 된다. 예를 들어, 기본 위치에 CPLEX-Python 모듈을 설치하려면 명령행에서 다음 명령을 사용하십시오.

(yourCplexhome/python/PLATFORM 디렉토리에서) python setup.py install

  이 명령을 사용하면 Python 패키지 distutils가 호출됩니다.

- 터미널(또는 cmd)에서 python 을 활성화시키고, import cplex 을 실행하여 CPLEX API가 호출되는지 확인한다.

- IBM CPLEX API 사용 방법 (Python 2.x 버전 기준 예제 ,원문)

위의 선형계획 문제가 주어질 경우, 이에 대한 Cplex Python API의 사용 소스코드는 다음과 같다.

## Step 1. CPLEX API 를 사용하기 위해서 해당 모듈을 가져온다.
import cplex

## Step 2. 선형계획 문제를 구성하기 위한 변수를 입력한다.( 이하의 모든 과정에서 값은 Float을 기본으로 한다. )
## 2-1. 각 변수의 이름을 순서대로 갖는 리스트
variables = ["x1", "x2", "x3"]

## 2-2. 목적식에서 각 변수의 계수를 순서대로 입력받은 리스트
obj_coeffs = [1.0, 2.0, 3.0] 

## 2-3. 각 변수의 상한값을 순서대로 갖는 리스트(상한값이 없을 경우, cplex.infinity 로 설정한다.)
upperbounds = [40.0, cplex.infinity, cplex.infinity] 

## 2-4. 각 변수의 하한값을 순서대로 갖는 리스트(하한값이 없을 경우, - cplex.infinity 로 설정한다.)
lowerbounds = [0.0, 0.0, 0.0] 

## 2-5. 제약조건의 이름을 순서대로 갖는 리스트
constraint_names = ["c1", "c2"]

## 2-6. 제약조건의 우변 상수를 순서대로 갖는 리스트
righthand = [20.0, 30.0]

## 2-7. 제약조건의 형태를 순서대로 갖는 리스트
## Greater than or equal to RHS: G
## Less than or equal to RHS: L   
## Equal to RHS: E   ## Given by range : R
senses = ['L', 'L']

## 2-8. 제약조건 순서대로, 각 조건의 계수 리스트를 담은 리스트. (아래의 형식을 지켜서 생성한다.)
lin_expr = [cplex.SparsePair(ind=["x1", "x2", "x3"], val=[-1.0, 1.0, 1.0]),
	       cplex.SparsePair(ind=["x1", "x2", "x3"], val=[1.0, -3.0, 1.0])]

## Step 3. 선형 계획 문제를 생성한다.
problem = cplex.Cplex()

## 3-1. 본 문제가 최대화 문제일 경우, problem.objective.sense.maximize
##        본 문제가 최소화 문제일 경우, problem.objective.sense.minimize 를 함수의 인자로 설정한다.
problem.objective.set_sense( problem.objective.sense.maximize )

## 3-2. 변수에 관한 정보를 입력한다.
## name = 이름 정보를 갖는 리스트
## obj = 목적식 계수 정보를 갖는 리스트
## ub = 상한값 정보를 갖는 리스트  ## lb = 하한값 정보를 갖는 리스트
problem.variables.add( obj=obj_coeffs, ub=upperbounds, lb=lowerbounds, names=variables )

## 3-3. 제약조건에 관한 정보를 입력한다.
## names = 제약조건 이름을 순서대로 갖는 리스트
## lin_expr = 선형 제약조건의 계수 정보를 순서대로 갖는 리스트
## senses = 제약조건의 형태를 순서대로 갖는 리스트
## rhs = 제약조건 우변 상수를 순서대로 갖는 리스트
problem.linear_constraints.add(lin_expr = lin_expr, senses = senses, rhs = righthand, names = constraint_names)

## Step 4. 솔버 함수를 실행한다.
problem.solve()

## Step 5. 결과물을 호출하여 출력한다.
numrows = problem.linear_constraints.get_num()
numcols = problem.variables.get_num()
## 5-1. 본 문제의 최적화 상태에 대한 정보( problem.solution.get_status() )를 출력한다. 
## Optimal 일 경우: = 1
## Unbound 일 경우: = 2
## Infeasible 일 경우: = 3
## Feasible 일 경우: = 23
## Infeasible or unbounded 일 경우: = 4
## 그 외 상태 코드에 대한 정보는 다음을 통해 참조 (링크)
print "Solution status = "+ repr(problem.solution.get_status())+ ": " +repr(problem.solution.status[problem.solution.get_status()])
## 5-2. 본 문제의 최적해(목적식)의 값을 출력한다. 
print "Solution value  = "+ repr(problem.solution.get_objective_value())

x = problem.solution.get_values()
shadow_price = problem.solution.get_dual_values()
## 5-3. 본 문제의 각 변수가 갖는 최적해를 출력한다. 
for i in range(numcols):
    print "Variable " + variables[i] + ": Value = " + repr(x[i])
## 5-4. 본 문제의 각 제약조건이 갖는 잠재가격(Shadow Price)를 출력한다.
for i in range(numrows):
    print "Constraint " + constraint_names[i] + ": Shadow Price = " + repr(shadow_price[i])

- 위의 설명을 토대로, 아래의 요구사항을 따르는 Python 스크립트를 작성한다.
선형계획 문제를 입력받았을때, 최적해와 최적해를 구성하는 각 변수의 값을 출력하는 스크립트를 작성한다.

각각의 문제는 Maximize 와 Minimize 두가지 문자열 중 하나로 시작된다.
각각의 문제에서 제약조건은 <=, >= 또는 = 세 가지로만 이루어 진다.
각각의 문제에서 선형식(Linear Expression) 부분은 공백(“ ”)을 포함하지 않을것이며, 공백은 등호, 부등호 그리고 Maximize/Minimize가 등장할 때만 사용될 것이다. 
숫자, Operator(+,-,*), Maximize/Minimize 그리고 공백문자를 제외한 나머지 알파벳은 변수 이름으로 사용될 것이며 변수명은 항상 한개의 문자(Character)로만 주어질 것이다. 답을 출력할 때, 해당 변수명 또한  일치 해야한다.
정수조건은 포함하지 않는다.
각각의 문제는 END라는 문자열로 종료된다.
CPLEX API 연습 문제이므로 해당 API의 모듈을 사용하여 문제를 해결하도록 한다.
평가를 위한 모든 인풋 문제는 하나의 해가 존재한다.(즉 문제 해결 후 상태코드는 항상 1이다.)
해가 존재하지 않는 다른 경우들에 대한 상태코드 처리 등은 개인적으로 실습해보도록한다.
목적식에는 문제가 포함하는 모든 변수가 사용된다. 즉, 목적식에 등장하지 않는 변수는 제약 조건에서 사용되지 않는다.
출력의 형식은 다음과 같다.
Problem #[문제번호]
Solution value  = [최적해]
Variable [첫번째 변수명]: Value = [값]
                        ... 
Variable [마지막 변수명]: Value = [값]
Constraint resource-1: Shadow Price = [값]
                        ... 
Constraint resource-[N]: Shadow Price = [값]
이 때, 제약조건에 하나의 변수만 존재할 경우 Upperbound 또는 Lowerbound로 처리하며, Resource 로 인식하지 않도록 처리한다.
그 외 예외 처리는 요구하지 않는다.

3. 힌트
Python Regular Expression
Python String Split with Multiple Delimiters
Python Regular Expression for Integers
Using Escape Sequence in Python

4. 입력파일의 형식과 예시
 입력 파일의 첫번째 줄에는 해결해야하는 선형계획 문제의 수인 N에 해당하는 자연수가 주어지고, 두번째 줄부터는 선형계획 문제에 대한 정보가 주어진다. 먼저 목적식에 해당하는 정보가 한줄 주어지며, 그 이후로 제약식이 주어진다. 마지막 제약식 이 후에는 “END”라는 문자열이 한줄에 주어져 해당 문제가 끝났음을 표시한다. 그 다음줄에서 다시 다음 문제의 목적식에 해당하는 정보가 주어지는 식이다.

예시>
3
Maximize a+2*b
a+3*b <= 8
a+b <= 4
a >= 0
b >= 0
END
Maximize 2*a+3*b
-3*a+b <= 1
4*a+2*b <= 20
4*a-b <= 10
a >= 0
b >= 0
END
Minimize 5*p+7*q
2*p+3*q >= 42
3*p+4*q >= 60
p+q >= 18
p >= 0
q >= 0
END

5. 출력파일의 형식과 예시
 문제번호를 #n 의 형태로 출력하고, 목표하는 수명 주기까지 먼저 몇번째 상태인지 state_i의 형태로 출력한 뒤 해당 상태를 입력파일에서의 형식과 마찬가지로 출력한다. 출력파일의 경로와 이름은 “./output.txt” 를 사용한다.
예시>
Problem #1
Solution value  = 6.0
Variable a: Value = 2.0
Variable b: Value = 2.0
Constraint resource-1: Shadow Price = 0.5
Constraint resource-2: Shadow Price = 0.5
Problem #2
Solution value  = 22.8
Variable a: Value = 1.7999999999999998
Variable b: Value = 6.4
Constraint resource-1: Shadow Price = 0.8
Constraint resource-2: Shadow Price = 1.1
Constraint resource-3: Shadow Price = -0.0
Problem #3
Solution value  = 102.0
Variable p: Value = 12.0
Variable q: Value = 6.0
Constraint resource-1: Shadow Price = 1.0
Constraint resource-2: Shadow Price = 1.0
Constraint resource-3: Shadow Price = -0.0
		
6. 평가
 사전에 알려지지 않은 임의의 입력 파일에 대해, 정답과 일치하는지 확인한다. 일치하지 않을 경우, 오답으로 간주한다.

		
"""
import sys
import operator

reload(sys)
sys.setdefaultencoding('UTF8')

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
	read_info = read_input("./Input_ProductionPlanning.xlsx")
	output = production_planning( read_info )
	print_output( output )
if __name__ == "__main__":
    main()

