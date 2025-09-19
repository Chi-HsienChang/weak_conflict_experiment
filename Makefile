SRCS = main.cpp eg.cpp chromosome.cpp problem.cpp weak.cpp utils.cpp

main: $(SRCS)
	g++ -std=c++20 $(SRCS) -o main

clean:
	rm -f main
