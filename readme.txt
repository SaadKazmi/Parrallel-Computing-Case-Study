##How to run the python code ##
########################################
python pmatrix.py minput.txt moutput.txt
########################################

##How to run the java code ##
########################################
javac javamatrix.java     
java javamatrix minput.txt moutput.txt 
######################################## 

##How to run the c code ##
########################################
gcc checkmatrix.c -o checkmatrix -lrt
./checkmatrix cinput.txt coutput.txt
########################################

##How to run the c vector code ##
########################################
gcc -o vectormatrix vectormatrix.c -msse4.1 -lrt
./vectormatrix cinput.txt coutput.txt
########################################

##How to run the c threads code ##
#######################################
gcc threadmatrix.c -o threadmatrix -lpthread -lrt
./threadmatrix cinput.txt coutput.txt
#######################################

##How to run the c vectors + threads code ##
#######################################
gcc -o vtmatrix  vtmatrix.c -msse4.1 -lpthread -lrt
./vtmatrix cinput.txt coutput.txt
#######################################

