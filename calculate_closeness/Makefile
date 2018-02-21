election: inverse_knapsack.o knapsack.o
	clang++ -o election -std=c++11 -stdlib=libc++ inverse_knapsack.o knapsack.o calculate_closeness.cpp

inverse_knapsack.o: knapsack.o
	clang++ -c -std=c++11 -stdlib=libc++ knapsack.o inverse_knapsack.cpp

knapsack.o:
	clang++ -c -std=c++11 -stdlib=libc++ knapsack.cpp

clean:
	rm *.o
