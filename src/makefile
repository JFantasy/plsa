CXX = g++ -std=c++11

plsa:main.o plsa.o
	$(CXX) -o $@ $^

main.o:main.cpp
	$(CXX) -c $^

plsa.o:plsa.cpp
	$(CXX) -c $^

clean:
	rm *.o plsa
