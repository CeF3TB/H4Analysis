INC_DIR = ./
CXX		=g++
LD		=g++
CXXFLAGS	=-O2 -ggdb -std=gnu++0x -Wall -fPIC -I/usr/include/mysql $(CFLAGS)
CXXFLAGSLINKDEF	=-fPIC
LDFLAGS		=-lz -lm -lMathMore
SOFLAGS		=-fPIC -shared 
SHELL		=bash
###
SrcSuf        = cpp
HeadSuf       = hpp
ObjSuf        = o
DepSuf        = d
DllSuf        = so
StatSuf       = a

## Packages	=controller testDataTypeServer testDataTypeClient testDataType testXMLLib testUInt
## Objects		=Daemon EventBuilder Handler Logger Profiler  Configurator ControlManager ConnectionManager Utility HwManager AsyncUtils BoardConfig

CppTestFiles=$(wildcard test/*.$(SrcSuf))
Packages=$(patsubst test/%.$(SrcSuf),%,$(CppTestFiles) )

CppSrcFiles=$(wildcard src/*.$(SrcSuf))
Objects=$(patsubst src/%.$(SrcSuf),%,$(CppSrcFiles))

LibName		=H4Analysis

### ----- OPTIONS ABOVE ----- ####

#InfoLine = \033[01\;31m compiling $(1) \033[00m
InfoLine = compiling $(1)

BASEDIR=$(shell pwd)
BINDIR=$(BASEDIR)/bin
DICTDIR=$(BASEDIR)/dict
LINKDIR=$(BASEDIR)/LinkDef
SRCDIR = $(BASEDIR)/src
HDIR = $(BASEDIR)/interface

Dict=$(patsubst src/%.$(SrcSuf),$(BASEDIR)/dict/%_Dict.$(ObjSuf),$(CppSrcFiles))
CXXFLAGS += "-I$(BASEDIR)"
CXXFLAGSLINKDEF += "-I$(BASEDIR)"
### Main Target, first
.PHONY: all
all: all2

######### ROOT
#include make/Makefile.ROOT
CXXFLAGS += `root-config --cflags`
CXXFLAGSLINKDEF += `root-config --cflags`
LDFLAGS += `root-config --libs`

#wait to update Packages in NO_xxx
.PHONY: all2
all2: info $(Packages) soLib | $(BINDIR)

BINOBJ	=$(patsubst %,$(BINDIR)/%.$(ObjSuf),$(Objects) )
SRCFILES=$(patsubst %,$(SRCDIR)/%.$(SrcSuf),$(Objects) )
HFILES	=$(patsubst %,$(HDIR)/%.$(HeadSuf),$(Objects) )
StatLib		=$(BINDIR)/$(LibName).$(StatSuf)
SoLib		=$(BINDIR)/$(LibName).$(DllSuf)

.PRECIOUS:*.ObjSuf *.DepSuf *.DllSuf

Deps=$(patsubst %,$(BINDIR)/%.$(DepSuf),$(Objects) $(Packages) )

############### EXPLICIT RULES ###############

$(BINDIR):
	mkdir -p $(BINDIR)
$(DICTDIR):
	mkdir -p $(DICTDIR)

info:
	@echo "--------------------------"
	@echo "Compile on $(shell hostname)"
	@echo "Packages are: $(Packages)"
	@echo "Objects are: $(Objects)"
	@echo "Dict are: $(Dict)"
	@echo "--------------------------"
	@echo "DEBUG:"

$(StatLib): $(BINOBJ) $(Dict)
	ar rcs $@ $(BINOBJ) $(Dict)
.PHONY: soLib
soLib: $(SoLib)

$(SoLib): $(StatLib) tag
	$(LD) $(LDFLAGS) $(SOFLAGS) -o $@  $(BINOBJ) $(Dict)

.PHONY: $(Packages) 
$(Packages): % : $(BINDIR)/% | $(BINDIR)
	@echo $(call InfoLine , $@ )

#$(BINDIR)/$(Packages): $(BINDIR)/% : $(BASEDIR)/test/%.$(SrcSuf) $(StatLib) | $(BINDIR)
$(addprefix $(BINDIR)/,$(Packages)): $(BINDIR)/% : $(BASEDIR)/test/%.$(SrcSuf) $(StatLib) | $(BINDIR)
	@echo $(call InfoLine , $@ )
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(StatLib) -I$(INC_DIR) -I$(HDIR)

#make this function of $(Packages)
#.PHONY: controller
#controller: bin/controller
#bin/controller: test/controller.cpp $(BINOBJ) $(StatLib)
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(BINOBJ) $< 


.PHONY: clean
clean:
	-rm -v bin/*.$(ObjSuf)
	-rm -v bin/*.$(DllSuf)
	-rm -v bin/*.$(DepSuf)
	-rm -v bin/*.$(StatSuf)
	-rm -v dict/*
	-rm -v $(addprefix $(BINDIR)/,$(Packages))


############### IMPLICIT RULES ###############


.PHONY: tag
tag: $(BINDIR)/tag.txt

.PHONY: $(BINDIR)/tag.txt
$(BINDIR)/tag.txt:
	git rev-parse HEAD > $(BINDIR)/tag.txt

#.o
%.$(ObjSuf): $(BINDIR)/%.$(ObjSuf)

#.o
$(BINDIR)/%.$(ObjSuf): $(SRCDIR)/%.$(SrcSuf) $(HDIR)/%.$(HeadSuf)
	@echo $(call InfoLine , $@ )
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $(SRCDIR)/$*.$(SrcSuf) -I$(INC_DIR) -I$(HDIR)

#.d
$(BINDIR)/%.$(DepSuf): $(SRCDIR)/%.$(SrcSuf) $(HDIR)/%.$(HeadSuf)
	@echo $(call InfoLine , $@ )
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -M -o $@ $(SRCDIR)/$*.$(SrcSuf) -I$(INC_DIR) -I$(HDIR)
	sed -i'' "s|^.*:|& Makefile $(BINDIR)/&|g" $@

$(DICTDIR)/%_Dict.cc:  $(SRCDIR)/%.$(SrcSuf) $(HDIR)/%.$(HeadSuf) | $(DICTDIR)
	@echo $(call InfoLine , $@ )
	cd $(DICTDIR) && rootcint -v4 -f $@ -c $(CXXFLAGSLINKDEF)  $(HDIR)/$*.$(HeadSuf) $(LINKDIR)/$*LinkDef.hpp

$(DICTDIR)/%_Dict.o:  $(DICTDIR)/%_Dict.cc
	@echo $(call InfoLine , $@ )
	cd $(DICTDIR) && $(CXX) -c -o $(DICTDIR)/$*_Dict.o $(DICTDIR)/$*_Dict.cc $(LDFLAGS) $(CXXFLAGS)
	
#-include $(Deps)
#	%.d: %.c
#		$(SHELL) -ec '$(CC) -M \
#			$(CPPFLAGS) $< | \
#			sed '\''s/$*.o/& $@/g'\'' \
#			> $@'
#	
