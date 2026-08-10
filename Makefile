#include Makefile.arch
#Makefile
BASEDIR = $(shell pwd)
SOURCEDIR = $(BASEDIR)/src
COREDIR = $(SOURCEDIR)/core
CONFIGDIR = $(SOURCEDIR)/configuration
ELOSSDIR = $(SOURCEDIR)/eloss
SIMDIR = $(SOURCEDIR)/simulation
PHYSDIR = $(SOURCEDIR)/physics
TREEDIR = $(SOURCEDIR)/tree
LIBDIR   = $(BASEDIR)/lib
OBJECTDIR = $(BASEDIR)/obj
BINARYDIR = $(BASEDIR)/bin
HEADER = -I$(COREDIR) -I$(CONFIGDIR) -I$(ELOSSDIR) -I$(SIMDIR) -I$(PHYSDIR) -I$(TREEDIR)

USE_TREEIRIS ?= 0

ROOTANA =
IRISDIR = 
IRISLIB =

ifeq ($(USE_TREEIRIS), 1)
# Path of ROOT analyzer
ROOTANA = $(HOME)/packages/rootana

#DPP tools for CAEN DT5740, added 2021-03-24
IRISDIR = /home/iris/packages/iris-daqtools
IRISLIB = $(IRISDIR)/build/libdaqtools_static.a
CXXFLAGS += -I$(ROOTANA) -I$(ROOTANA)/include
CXXFLAGS += -I$(IRISDIR)/include
CXXFLAGS += -Df2cFortran
endif

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
ROOTGLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lThread -Wl,-rpath,$(ROOTSYS)/lib
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

SETUPTCSH = $(BASEDIR)/setupIRIStcsh.sh
SETUPBASH = $(BASEDIR)/setupIRISbash.sh

TARGETS = $(BINARYDIR)/simIris $(BINARYDIR)/physIris 

ifeq ($(USE_TREEIRIS), 1)
TARGETS += $(BINARYDIR)/treeIris
endif

all: $(TARGETS) $(SETUPTCSH) $(SETUPBASH)

$(BINARYDIR)/simIris: $(OBJECTDIR)/simIris.o $(OBJECTDIR)/HandleSIMULATE.o $(OBJECTDIR)/dwba.o $(OBJECTDIR)/shieldClear.o $(LIBDIR)/libIRISCore.so $(LIBDIR)/libIRISConfig.so $(LIBDIR)/libIRISSim.so $(LIBDIR)/libIRISELoss.so
	$(CXX) -o $@ $(CXXFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -lm -lz -lutil -lpthread -lrt

$(BINARYDIR)/physIris: $(OBJECTDIR)/physIris.o $(OBJECTDIR)/HandlePHYSICS.o $(LIBDIR)/libIRISCore.so $(LIBDIR)/libIRISConfig.so $(LIBDIR)/libIRISELoss.so
	$(CXX) -o $@ $(CXXFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -lm -lz -lutil -lpthread -lrt

$(BINARYDIR)/treeIris: $(OBJECTDIR)/HandleMesytec.o $(OBJECTDIR)/HandleV1190.o $(OBJECTDIR)/HandleSTAT.o $(OBJECTDIR)/HandleScaler.o $(OBJECTDIR)/HandleV1740.o $(OBJECTDIR)/treeIris.o $(LIBDIR)/libIRISCore.so $(LIBDIR)/libIRISConfig.so $(ROOTANA)/lib/librootana.a $(IRISLIB)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(ROOTGLIBS) $(IRISLIB) -lm -lz -lutil -lpthread -lrt 

#remove -lnsl and -lrt for macOS
$(LIBDIR)/libIRISCore.so: $(OBJECTDIR)/IDet.o $(OBJECTDIR)/TEvent.o $(OBJECTDIR)/ITdc.o $(OBJECTDIR)/IScaler.o $(OBJECTDIR)/IRISCoreDict.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(ROOTGLIBS) -o $@
	@echo "$@ done"

$(LIBDIR)/libIRISConfig.so: $(OBJECTDIR)/CalibPHYSICS.o $(OBJECTDIR)/CalibMesytec.o $(OBJECTDIR)/geometry.o $(OBJECTDIR)/runDepPar.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(ROOTGLIBS) -o $@
	@echo "$@ done"

$(LIBDIR)/libIRISSim.so: $(OBJECTDIR)/PTrack.o $(OBJECTDIR)/IRISHit.o $(OBJECTDIR)/YYHit.o $(OBJECTDIR)/CsIHit.o $(OBJECTDIR)/S3Hit.o $(OBJECTDIR)/IRISSimDict.o $(LIBDIR)/libIRISELoss.so
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -o $@
	@echo "$@ done"

$(LIBDIR)/libIRISELoss.so: $(OBJECTDIR)/nucleus.o $(OBJECTDIR)/dedx.o $(OBJECTDIR)/eloss.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(CATIMALIBDIR) $(ROOTGLIBS) $(CATIMALIBS) -o $@
	@echo "$@ done"

$(OBJECTDIR)/simIris.o: $(SIMDIR)/mainSim.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/physIris.o: $(PHYSDIR)/mainPhys.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/treeIris.o: $(TREEDIR)/mainTree.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/IDet.o: $(COREDIR)/IDet.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/ITdc.o: $(COREDIR)/ITdc.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IScaler.o: $(COREDIR)/IScaler.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/TEvent.o: $(COREDIR)/TEvent.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CalibPHYSICS.o: $(CONFIGDIR)/CalibPHYSICS.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/runDepPar.o: $(CONFIGDIR)/runDepPar.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/geometry.o: $(CONFIGDIR)/geometry.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CalibMesytec.o: $(CONFIGDIR)/CalibMesytec.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IRISHit.o: $(SIMDIR)/IRISHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/YYHit.o: $(SIMDIR)/YYHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CsIHit.o: $(SIMDIR)/CsIHit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/S3Hit.o: $(SIMDIR)/S3Hit.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/PTrack.o: $(SIMDIR)/PTrack.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/eloss.o: $(ELOSSDIR)/eloss.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/dedx.o: $(ELOSSDIR)/dedx.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/nucleus.o: $(ELOSSDIR)/nucleus.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleSIMULATE.o: $(SIMDIR)/HandleSIMULATE.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/shieldClear.o: $(SIMDIR)/shieldClear.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/dwba.o: $(SIMDIR)/dwba.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandlePHYSICS.o: $(PHYSDIR)/HandlePHYSICS.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleMesytec.o: $(TREEDIR)/HandleMesytec.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleV1190.o: $(TREEDIR)/HandleV1190.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleV1740.o: $(TREEDIR)/HandleV1740.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleSTAT.o: $(TREEDIR)/HandleSTAT.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/HandleScaler.o: $(TREEDIR)/HandleScaler.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IRISCoreDict.o: $(LIBDIR)/IRISCoreDict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IRISSimDict.o: $(LIBDIR)/IRISSimDict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBDIR)/IRISCoreDict.cxx: $(COREDIR)/TEvent.h $(COREDIR)/IDet.h $(COREDIR)/ITdc.h $(COREDIR)/IScaler.h $(COREDIR)/IRISCoreLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(HEADER) $^

$(LIBDIR)/IRISSimDict.cxx: $(SIMDIR)/PTrack.h $(SIMDIR)/IRISHit.h $(SIMDIR)/YYHit.h $(SIMDIR)/CsIHit.h $(SIMDIR)/S3Hit.h $(SIMDIR)/IRISSimLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(HEADER) $(CATIMAINC) $(CATIMADEF) $^

$(SETUPTCSH): Makefile
	@echo '#!/bin/tcsh' > $@
	@echo '' >> $@
	@echo 'setenv IRISROOT $(BASEDIR)' >> $@
	@echo '' >> $@
	@echo 'if ( $$?ROOT_INCLUDE_PATH ) then' >> $@
	@echo '	setenv ROOT_INCLUDE_PATH $${ROOT_INCLUDE_PATH}:$(SOURCEDIR)/core:$(SOURCEDIR)/configuration:$(SOURCEDIR)/eloss:$(SOURCEDIR)/physics:$(SOURCEDIR)/simulation:$(SOURCEDIR)/tree' >> $@
	@echo 'else' >> $@
	@echo '	setenv ROOT_INCLUDE_PATH $(SOURCEDIR)/core:$(SOURCEDIR)/configuration:$(SOURCEDIR)/eloss:$(SOURCEDIR)/physics:$(SOURCEDIR)/simulation:$(SOURCEDIR)/tree' >> $@
	@echo 'endif' >> $@
	@echo '' >> $@
	@echo 'setenv LD_LIBRARY_PATH $${LD_LIBRARY_PATH}:$(LIBDIR)' >>$@
	@chmod +x $@
	@echo 'Generated $@'

$(SETUPBASH): Makefile
	@echo '#!/bin/bash' > $@
	@echo '' >> $@
	@echo 'export IRISROOT=$(BASEDIR)' >> $@
	@echo '' >> $@
	@echo 'export ROOT_INCLUDE_PATH=$${ROOT_INCLUDE_PATH}:$(SOURCEDIR)/core:$(SOURCEDIR)/configuration:$(SOURCEDIR)/eloss:$(SOURCEDIR)/physics:$(SOURCEDIR)/simulation:$(SOURCEDIR)/tree' >> $@
	@echo 'export LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$(LIBDIR)' >>$@
	@chmod +x $@
	@echo 'Generated $@'

clean::
	rm -f $(OBJECTDIR)/*.o
	rm -f $(BINARYDIR)/simIris
	rm -f $(BINARYDIR)/physIris
	rm -f $(BINARYDIR)/treeIris
	rm -f $(LIBDIR)/*Dict.cxx
	rm -f $(LIBDIR)/*Dict.h
	rm -f $(LIBDIR)/*.pcm
	rm -f $(LIBDIR)/*.so

# end 
