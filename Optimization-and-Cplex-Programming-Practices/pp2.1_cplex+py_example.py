# -*- coding: utf-8 -*-
"""
 Title: Cplex example (한글 활용 주석)

 1. 학습 목표: Cplex API 활용법을 익히고, 복수의 LP 문제를 자동화하여 해를 제시하는 프로그램을 구현한다.
 2. 설명:
	- IBM CPLEX Studio 설치 요령
	(1) IBM CPLEX Optimization Studio, Community Edition 다운로드 및 설치
		- http://www-01.ibm.com/software/websphere/products/optimization/cplex-studio-community-edition/
		- 변수 1000개, 제약조건 1000개 이하의 선형계획 문제까지 처리 가능한 버전.
		- 각자의 운영체제에 맞추어 설치할 것.
	(2) CPLEX의 Python API 설정(원문)
		- CPLEX의 Python API는 IBM ILOG CPLEX Optimization Studio의 일부이다. CPLEX Python API와 연관된 모듈은 yourCPLEXhome/python/PLATFORM 디렉토리(또는 yourCPLEXhome\python\PLATFORM 폴더)에 있다. 여기서, yourCPLEXhome은 IBM ILOG CPLEX Optimization Studio의 일부로 CPLEX가 설치된 위치를 지정하며 PLATFORM은 운영 체제와 컴파일러의 조합을 나타낸다.
		- 시스템에 CPLEX-Python 모듈을 설치하려면 yourCplexhome/python/PLATFORM에 있는 setuy.py 스크립트를 사용한다. 기본이 아닌 위치에 CPLEX-Python 모듈을 설치하려면 --home 옵션을 사용하여 설치 디렉토리를 식별하면 된다. 예를 들어, 기본 위치에 CPLEX-Python 모듈을 설치하려면 명령행에서 다음 명령을 사용한다. (yourCplexhome/python/PLATFORM 디렉토리에서) python setup.py install
		- 터미널(또는 cmd)에서 python 을 활성화시키고, import cplex 을 실행하여 CPLEX API가 호출되는지 확인한다.
		- IBM CPLEX API 사용 방법 (Python 2.x 버전 기준 예제 ,원문)

 3. 사용 예제
	http://www.ibm.com/support/knowledgecenter/ko/SSSA5P_12.6.0/ilog.odms.cplex.help/CPLEX/GettingStarted/topics/tutorials/ataglance/conv_prob_statement.html
"""

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
