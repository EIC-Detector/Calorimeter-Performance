AUTOMAKE_OPTIONS = foreign

AM_CXXFLAGS=-std=c++0x

# List of shared libraries to produce
lib_LTLIBRARIES = \
  libg4ana_calo.la

INCLUDES = \
  -I$(includedir) \
  -isystem $(OFFLINE_MAIN)/include \
  -isystem $(ROOTSYS)/include

RINCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include

libg4ana_calo_la_SOURCES = \
  G4AnaCalo_Dict.C \
  G4CaloShowerAnalysis.cc \
  G4CaloTowerAnalysis.cc


libg4ana_calo_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libg4ana_calo_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lg4detectors \
  -lgsl \
  -lgslcblas

G4AnaCalo_Dict.C: \
  G4CaloShowerAnalysis.h \
  G4CaloTowerAnalysis.h \
  G4AnaCaloLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(RINCLUDES) $^

#################################################
#### please add new classes in alphabetical order
###
###pkginclude_HEADERS = \
###  RawCluster.h \
###  RawClusterContainer.h \
###  RawTower.h \
###  RawTowerv1.h \
###  RawTowerv2.h \
###  RawTowerContainer.h \
###  RawTowerGeom.h
###

################################################
# linking tests

noinst_PROGRAMS = \
  testexternals_g4ana 

testexternals_g4ana_SOURCES = testexternals.C
testexternals_g4ana_LDADD = libg4ana_calo.la


testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

##############################################
# please add new classes in alphabetical order

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)
