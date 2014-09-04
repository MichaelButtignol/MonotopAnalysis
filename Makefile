ObjSuf        = o
SrcSuf        = cc
LogSuf        = log
ExeSuf        = run

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = 

CXX           = g++
CXXFLAGS      = -std=c++0x
CXXFLAGS      += -O -Wall -fPIC $(DEFINES) -Wno-unused-result -Wshadow
CXXFLAGS      += $(ROOTCFLAGS) -I./
CXXFLAGS      += -I../sonicScrewdriver/

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA
LDFLAGS       += -l:`pwd`/.vectorDictionnary_C.so
LDFLAGS       += -l:`pwd`/../sonicScrewdriver/libSonicScrewdriver.so

SOFLAGS       = -shared
LIBS          = 

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=.$(ExeSuf))
LOGS          = $(SOURCES:.$(SrcSuf)=.$(LogSuf))
#------------------------------------------------------------------------------

all: $(EXECUTABLES)

test: plots

# Target to produce the example plots
plots: plotGenerationTool.$(ExeSuf)
	@./plotGenerationTool.$(ExeSuf) | tee plotGenerationTool.$(LogSuf)

# Target to browse the plots
browse:
	PLOTS_TO_BROWSE=`ls plots/*.root` root -l .browse

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES) $(LOGS)

./.vectorDictionnary_C.so: ./.vectorDictionnary.C
	root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) ./.vectorDictionnary_C.so
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(LIBS) $(GCCPARSER)

.PHONY : plots test browse clean









