CC = g++
CFLAGS = -g -Wall --std=c++11

smoketest: unit.cpp ../src/utf8.h
	$(CC) $(CFLAGS) unit.cpp -ounit -lboost_unit_test_framework
	./unit
