AUTOMAKE_OPTIONS = foreign

# List of shared libraries to produce
lib_LTLIBRARIES = \
    libg4ana_calorimeter.la

AM_LDFLAGS = \
    -L$(libdir) \
    -L$(OFFLINE_MAIN)/lib

INCLUDES = \
    -I$(includedir) \
    -I$(OFFLINE_MAIN)/include \
    -I$(ROOTSYS)/include \
    -I${G4_MAIN}/include \
    -I${G4_MAIN}/include/Geant4

libg4ana_calorimeter_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lg4testbench \
  -lCGAL

pkginclude_HEADERS = \
  G4CaloShowerAnalysis.h \
  G4CaloTowerAnalysis.h

libg4ana_calorimeter_la_SOURCES = \
  G4CaloShowerAnalysis.cc \
  G4CaloTowerAnalysis.cc

################################################
# linking tests

noinst_PROGRAMS = \
  testexternals_g4ana_calorimeter

testexternals_g4ana_calorimeter_SOURCES = testexternals.cc
testexternals_g4ana_calorimeter_LDADD = libg4ana_calorimeter.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

##############################################
# please add new classes in alphabetical order

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)

G4Analysis_Calorimeter_Dict.cc : \
  G4CaloShowerAnalysis.h \
  G4CaloTowerAnalysis.h \
  G4CaloAnalysisLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

