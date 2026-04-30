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

all:  $(BINARYDIR)/simIris

$(BINARYDIR)/simIris: $(OBJECTDIR)/simIris.o $(OBJECTDIR)/nucleus.o $(OBJECTDIR)/reacParams.o $(OBJECTDIR)/geoParams.o $(OBJECTDIR)/dedx.o $(OBJECTDIR)/dwba.o $(OBJECTDIR)/EnergyLossManager.o $(OBJECTDIR)/shieldClear.o $(LIBDIR)/libSimEvent.so $(OBJECTDIR)/SimEventDict.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -lm -lz -lutil -lpthread -lrt
#remove -lnsl and -lrt for macOS
$(LIBDIR)/libSimEvent.so: $(OBJECTDIR)/PTrack.o $(OBJECTDIR)/YYHit.o $(OBJECTDIR)/IPhys.o $(OBJECTDIR)/CsIHit.o $(OBJECTDIR)/S3Hit.o $(OBJECTDIR)/IDet.o $(OBJECTDIR)/SimEventDict.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $(ROOTGLIBS) $^ -o $@
	@echo "$@ done"

$(OBJECTDIR)/simIris.o: $(SOURCEDIR)/main.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/reacParams.o: $(SOURCEDIR)/reacParams.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/geoParams.o: $(SOURCEDIR)/geoParams.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/nucleus.o: $(SOURCEDIR)/nucleus.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/dedx.o: $(SOURCEDIR)/dedx.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/dwba.o: $(SOURCEDIR)/dwba.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/EnergyLossManager.o: $(SOURCEDIR)/EnergyLossManager.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/shieldClear.o: $(SOURCEDIR)/shieldClear.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IDet.o: $(SOURCEDIR)/IDet.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/YYHit.o: $(SOURCEDIR)/YYHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IPhys.o: $(SOURCEDIR)/IPhys.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CsIHit.o: $(SOURCEDIR)/CsIHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/S3Hit.o: $(SOURCEDIR)/S3Hit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/PTrack.o: $(SOURCEDIR)/PTrack.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/SimEventDict.o: $(LIBDIR)/SimEventDict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBDIR)/SimEventDict.cxx:  $(INCLUDEDIR)/PTrack.h $(INCLUDEDIR)/YYHit.h $(INCLUDEDIR)/IPhys.h $(INCLUDEDIR)/CsIHit.h $(INCLUDEDIR)/S3Hit.h $(INCLUDEDIR)/IDet.h $(INCLUDEDIR)/SimEventLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(HEADER) $(CATIMAINC) $(CATIMADEF) $^

clean::
	rm -f $(OBJECTDIR)/*.o
	rm -f $(BINARYDIR)/simIris
	rm -f $(LIBDIR)/*Dict.cxx
	rm -f $(LIBDIR)/*Dict.h
	rm -f $(LIBDIR)/*.pcm
	rm -f $(LIBDIR)/libSimEvent.so

# end 
