CC=g++


CSRCS=my-cpp-test.cpp
TARGETS=cpp-test

#BUFL=-L /opt/net-snmp/lis_d -lnetsnmpagent -lnetsnmpmibs -lnetsnmphelpers -lnetsnmp 


CFLAGS= -Wall #-std=c++11
#INC=-I /opt/net-snmp/include

all:
	$(CC) $(CFLAGS) $(CSRCS) -o $(TARGETS) 

clean:
	rm -f  $(TARGETS) *.o
