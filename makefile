OBJS = game.cpp WinManager.cpp helperFunctions.cpp Point.cpp Ship.cpp
HEADERS = WinManager.h helperFunctions.h constants.h Point.h Ship.h

all : $(OBJS) $(HEADERS)
	g++ $(OBJS)
