#include Makefile.arch
#Makefile
BASEDIR = $(shell pwd)
SOURCEDIR = $(BASEDIR)/src
INCLUDEDIR = $(BASEDIR)/include
LIBDIR   = $(BASEDIR)/lib
OBJECTDIR = $(BASEDIR)/obj
BINARYDIR = $(BASEDIR)/bin
HEADER = -I$(INCLUDEDIR) #-I$(TREEIRIS)/include

USE_CATIMA ?= 0

CATIMAPATH ?= /home/iris/curtis/NewIris/catima/install
CATIMALIBDIR =
CATIMALIBS =
CATIMAINC =
CATIMADEF =

ifeq ($(USE_CATIMA), 1)
CATIMALIBDIR = -L$(CATIMAPATH)/lib
CATIMALIBS = -Wl,-rpath,$(CATIMAPATH)/lib -lcatima
CATIMAINC = -isystem $(CATIMAPATH)/include
CATIMADEF = -DUSE_CATIMA
endif

CXX = g++
LD = g++
ifdef ROOTSYS
ROOTGLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs) -lThread -Wl,-rpath,$(ROOTSYS)/lib
CXXFLAGS += -g -O -Wall -Wuninitialized -I./ -I$(ROOTSYS)/include
ROOTCFLAGS    = $(shell root-config --cflags)
CXXFLAGS += $(HEADER)
CXXFLAGS      += -g -ansi -fPIC $(ROOTCFLAGS)
CXXFLAGS += $(CATIMAINC) $(CATIMADEF)
endif 


SOFLAGS = -g -shared
LDFLAGS = -O2

#use these flags for macOS
#SOFLAGS       = -g -dynamiclib -shared
#LDFLAGS       = -O2 -undefined dynamic_lookup

all:  $(BINARYDIR)/simIris $(BINARYDIR)/physIris

$(BINARYDIR)/simIris: $(OBJECTDIR)/simIris.o $(OBJECTDIR)/HandleSIMULATE.o $(OBJECTDIR)/dwba.o $(OBJECTDIR)/shieldClear.o $(LIBDIR)/libIRISEvent.so
	$(CXX) -o $@ $(CXXFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -lm -lz -lutil -lpthread -lrt

$(BINARYDIR)/physIris: $(OBJECTDIR)/physIris.o $(OBJECTDIR)/HandlePHYSICS.o $(OBJECTDIR)/Graphsdedx.o $(LIBDIR)/libIRISEvent.so
	$(CXX) -o $@ $(CXXFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -lm -lz -lutil -lpthread -lrt

#remove -lnsl and -lrt for macOS
$(LIBDIR)/libIRISEvent.so: $(OBJECTDIR)/TEvent.o $(OBJECTDIR)/ITdc.o $(OBJECTDIR)/nucleus.o $(OBJECTDIR)/dedx.o $(OBJECTDIR)/PTrack.o $(OBJECTDIR)/IRISHit.o $(OBJECTDIR)/YYHit.o $(OBJECTDIR)/CsIHit.o $(OBJECTDIR)/S3Hit.o $(OBJECTDIR)/IDet.o  $(OBJECTDIR)/IScaler.o $(OBJECTDIR)/eloss.o $(OBJECTDIR)/runDepPar.o $(OBJECTDIR)/CalibPHYSICS.o $(OBJECTDIR)/geometry.o $(OBJECTDIR)/IRISEventDict.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) $^ -o $@
	@echo "$@ done"

$(OBJECTDIR)/simIris.o: $(SOURCEDIR)/mainSim.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/physIris.o: $(SOURCEDIR)/mainPhys.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/nucleus.o: $(SOURCEDIR)/nucleus.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/dedx.o: $(SOURCEDIR)/dedx.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/dwba.o: $(SOURCEDIR)/dwba.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/shieldClear.o: $(SOURCEDIR)/shieldClear.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IDet.o: $(SOURCEDIR)/IDet.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IRISHit.o: $(SOURCEDIR)/IRISHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/YYHit.o: $(SOURCEDIR)/YYHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CsIHit.o: $(SOURCEDIR)/CsIHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/S3Hit.o: $(SOURCEDIR)/S3Hit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/PTrack.o: $(SOURCEDIR)/PTrack.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandlePHYSICS.o: $(SOURCEDIR)/HandlePHYSICS.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleSIMULATE.o: $(SOURCEDIR)/HandleSIMULATE.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/eloss.o: $(SOURCEDIR)/eloss.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/runDepPar.o: $(SOURCEDIR)/runDepPar.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/geometry.o: $(SOURCEDIR)/geometry.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/Graphsdedx.o: $(SOURCEDIR)/Graphsdedx.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CalibPHYSICS.o: $(SOURCEDIR)/CalibPHYSICS.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/ITdc.o: $(SOURCEDIR)/ITdc.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IScaler.o: $(SOURCEDIR)/IScaler.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/TEvent.o: $(SOURCEDIR)/TEvent.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IRISEventDict.o: $(LIBDIR)/IRISEventDict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBDIR)/IRISEventDict.cxx: $(INCLUDEDIR)/TEvent.h $(INCLUDEDIR)/PTrack.h $(INCLUDEDIR)/IRISHit.h $(INCLUDEDIR)/YYHit.h $(INCLUDEDIR)/CsIHit.h $(INCLUDEDIR)/S3Hit.h $(INCLUDEDIR)/IDet.h $(INCLUDEDIR)/ITdc.h $(INCLUDEDIR)/IScaler.h $(INCLUDEDIR)/IRISEventLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(HEADER) $(CATIMAINC) $(CATIMADEF) $^

clean::
	rm -f $(OBJECTDIR)/*.o
	rm -f $(BINARYDIR)/simIris
	rm -f $(BINARYDIR)/physIris
	rm -f $(LIBDIR)/*Dict.cxx
	rm -f $(LIBDIR)/*Dict.h
	rm -f $(LIBDIR)/*.pcm
	rm -f $(LIBDIR)/libIRISEvent.so

# end 
