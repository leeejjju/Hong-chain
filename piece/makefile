CC = g++
IP = 127.0.0.1
PORT = 9090
CFLAGS = -pthread -I/usr/include/jsoncpp -lcurl -ljsoncpp
all: demon.out


clnt.out: client.cpp
	$(CC) $^ -o $@

demon.out: demon.cpp
	$(CC) $^ -o $@ $(CFLAGS) 

rc: clnt.out
#	./$^ $(IP) $(PORT) 22100579
	python3 action.py $(IP) $(PORT) "SUBMIT:22100579"

rs: demon.out
	./$^ $(PORT)
	
clean:
	rm -f *.out
	rm -rf submissions
	rm -f solution.cpp
	rm -f test_driver.cpp
