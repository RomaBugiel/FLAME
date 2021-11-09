CXX=`root-config --cxx`
CXXFLAGS=`root-config --cflags` 
LDFLAGS=`root-config --ldflags`

LDLIBS = `root-config --glibs` 

SOURCES= src/ClusterFormation.cpp src/HistoPlotter.cpp src/TreeOnFireReader.cpp  src/RunControl.cpp 

HEADERS=/data_structures.h /globals.h /cout_msh.h /plane_str.h 
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=run_flame_clusterization

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(SOURCES) 
	$(CXX) $(CXXFLAGS) -O2 -W $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LDLIBS) 

#.cc.o:
	# $(CXX) $(CXXFLAGS) -W -Wall -c $<

clean:
	rm ./*~ ./*.o ./*.so ./run_flame_clusterization ./AutoDict* ./*.pcm
	
	 
docs:
	doxygen Doxyfile

#`root-config --cxx` `root-config --cflags`  -O2 -W TreeOnFireReader.cpp -o offline_analysis `root-config --ldflags` `root-config --glibs`

linkdef: 
	rootcint FlameDict.cpp -c data_structures.h  FlameLinkDef.h
