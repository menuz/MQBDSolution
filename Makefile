CC=g++
OBJS=instr.o DBUtil.o MQBDSolution.o
LIB=-L/opt/oracle/oracle11g/product/11.2.0/dbhome_1/lib -L/opt/oracle/oracle11g/product/11.2.0/dbhome_1/rdbms/lib/ 
INCLUDE=-I/opt/oracle/oracle11g/product/11.2.0/dbhome_1/precomp/public -I/opt/oracle/oracle11g/product/11.2.0/dbhome_1/rdbms/public 
MQBDSolution: $(OBJS)
	$(CC) -o MQBDSolution $(OBJS) $(LIB) -pthread -locci -lclntsh 
instr.o: include/instr.cpp 
	$(CC) -c include/instr.cpp 
DBUtil.o: dao/DBUtil.cpp
	$(CC) -c  dao/DBUtil.cpp $(INCLUDE)
MQBDSolution.o:MQBDSolution.cpp
	$(CC) -c MQBDSolution.cpp $(INCLUDE) -Wall -O -g 
clean:
	rm -rf *.o  & rm MQBDSolution
