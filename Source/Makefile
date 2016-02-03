OBJECTS = MyTools.o Circle.o Node.o MyFeatureDetector.o main.o
TESTINGOBJECTS = MyTools.o Circle.o Node.o MyFeatureDetector.o testall.o
PROGRAM = executable
TESTING = test
CXX ?= g++
DEBUG = -g

CXXFLAGS += -c -Wall $(shell pkg-config --cflags opencv)
LDFLAGS += $(shell pkg-config --libs --static opencv)

all: executable test

executable: $(OBJECTS); $(CXX) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)

test: $(TESTINGOBJECTS); $(CXX) $(TESTINGOBJECTS) -o $(TESTING) $(LDFLAGS)


# mCenek's code for compiling multiple files at the same time
MyTools.o: MyTools.cpp
	$(CXX) $(CXXFLAGS) MyTools.cpp

Circle.o: Circle.cpp
	$(CXX) $(CXXFLAGS) Circle.cpp

Node.o: Node.cpp
	$(CXX) $(CXXFLAGS) Node.cpp

MyFeatureDetector.o: MyFeatureDetector.cpp
	$(CXX) $(CXXFLAGS) MyFeatureDetector.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

testall.o: testall.cpp
	$(CXX) $(CXXFLAGS) testall.cpp

# opencv example code
# %.o: %.cpp; $(CXX) $< -o $@ $(CXXFLAGS)

clean: ; rm -f MyTools.o Circle.o Node.o MyFeatureDetector.o main.o executable test.o test
