OBJS = game.cpp WinManager.cpp helperFunctions.cpp Point.cpp
HEADERS = WinManager.h helperFunctions.h constants.h Point.h

all : $(OBJS) $(HEADERS)
	g++ $(OBJS)
