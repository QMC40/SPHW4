###########################################################
# Aaron Fortner                                           #
# COSC4348-001                                            #
# Systems Programming                                     #
# Assignment 3                                            #
###########################################################

compiler = gcc
ExePath = ./exec/
SourcePath1 = ./vers1/
SourcePath2 = ./problem_2/
VPATH = ./vers1 ./problem_2 ./exec
FLAGS = -g -Wall -o

target: ./exec/vers1 
#./exec/problem2

$(ExePath)vers1: main.c medianator.h copier.h
	@mkdir -p exec
	$(compiler) $(FLAGS) $(ExePath)vers1 $(SourcePath1)main.c \ 
	medianator.h copier.h

$(ExePath)problem2: problem_2.c copyfile.h
	@mkdir -p exec
	$(compiler) $(FLAGS) $(ExePath)problem2 $(SourcePath2)problem_2.c $(SourcePath2)copyfile.h

run: clearscreen
	@echo "\nProblem 1 - child exit status\n"
	@$(ExePath)problem1
	@echo "\nProblem 2 - Read / Write time vs buffer size demo\n"
	@$(ExePath)problem2 $(SourcePath2)jobsource.txt $(ExePath)jobcopy.txt

.PHONY: clean

clean:
	@-rm $(ExePath)*
	@-rmdir ./exec

#target that compiles source code as needed, displays
#status, clears the screen then runs code then deletes files
script1: target clearscreen run clean

#phony target to allow calling of 'clear'
.PHONY: clearscreen

#'sleep' delays screen clear long enough to see any status msg from make
#such as if pre-reqs are current and no recompile needed
clearscreen:
	@sleep 1
	@clear
	
.PHONY: module load

module load:
	module load gcc
