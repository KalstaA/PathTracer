
output: hello.o message.o
	g++ hello.o message.o -o output

hello.o: hello.cpp
	g++ -c hello.cpp

message.o: message.cpp
	g++ -c message.cpp

clean:
	rm *.o output


