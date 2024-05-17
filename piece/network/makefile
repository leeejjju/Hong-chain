CC = g++
IP = 127.0.0.1
PORT = 9090

all: clnt.out serv.out


clnt.out: client.cpp
	$(CC) $^ -o $@

serv.out: demon.cpp
	$(CC) $^ -o $@

rc: clnt.out
#	./$^ $(IP) $(PORT) 22100579
	python3 action.py $(IP) $(PORT) "SUBMIT:22100579"

rs: serv.out
	./$^ $(PORT)
	
clean:
	rm -f *.out
	rm -rf submissions
	rm -f solution.cpp
	rm -f test_driver.cpp
