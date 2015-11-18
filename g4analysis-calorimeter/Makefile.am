AUTOMAKE_OPTIONS = foreign

INCLUDES = \
  -I$(includedir) \
  -isystem $(OFFLINE_MAIN)/include/eigen3 \
  -isystem $(OFFLINE_MAIN)/include \
  -isystem $(G4_MAIN)/include \
  -isystem $(ROOTSYS)/include \
  -isystem /direct/phenix+u/nfeege/sphenixsw/devel/install/g4cemc/include

RINCLUDES = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include/eigen3 \
  -I$(OFFLINE_MAIN)/include \
  -I$(G4_MAIN)/include \
  -I$(ROOTSYS)/include \
  -I/direct/phenix+u/nfeege/sphenixsw/devel/install/g4cemc/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

AM_CXXFLAGS =  -msse2

lib_LTLIBRARIES = libg4ana_calo.la

libg4ana_calo_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  `geant4-config --libs` \
  -lfun4all \
  -lg4hough \
  -lHelixHough \
  -lFitNewton \
  -lcemc

pkginclude_HEADERS = \
  G4CaloShowerAnalysis.h \
  G4CaloTowerAnalysis.h

#pkginclude_HEADERS = $(include_HEADERS)

libg4ana_calo_la_SOURCES = \
  G4AnaCalo_Dict.C \
  G4CaloShowerAnalysis.cc \
  G4CaloTowerAnalysis.cc

G4AnaCalo_Dict.C : \
  G4CaloShowerAnalysis.h \
  G4CaloTowerAnalysis.h \
  G4CaloAnalysisLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(RINCLUDES) $^

################################################
# linking tests

noinst_PROGRAMS = testexternals

BUILT_SOURCES = \
  testexternals.C

testexternals_LDADD = \
  libg4ana_calo.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict*
