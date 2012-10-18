CXXFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lpsmoveapi -lpsmoveapi_tracker
all:
	g++ *.cpp $(CXXFLAGS) -o movebreaker


