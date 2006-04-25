#-----------------------------------------------------------
# GOGGLES BUILD SYSTEM
#-----------------------------------------------------------
#
# The actual make file
#
#-----------------------------------------------------------

# Convert to Platform specific names
#----------------------------------------------------------
LIBNAME=src/$(LIBPREFIX)$(PACKAGE_NAME)$(LIBEXT) 		# libXXX.a on Linux, XXX.lib on Windows
BINNAME=src/$(PACKAGE_NAME)$(BINEXT)    						# XXX on Linux, X.exe on Windows

all:  $(BINNAME)

.PHONY : clean

# Objects to Compile
#----------------------------------------------------------
SRCFILES := $(sort $(wildcard src/*.cpp))
OBJECTS :=  $(patsubst %.cpp,%$(OBJEXT),$(SRCFILES))

#----------------------------------------------------------

$(BINNAME): $(OBJECTS)
	@echo "    Linking $@ ..."
#	@echo "$(LINK) $(LDFLAGS) $(OUTPUTBIN)$(BINNAME) $(OBJECTS) $(LIBS)"
	@$(LINK) $(LDFLAGS) $(OUTPUTBIN)$(BINNAME) $(OBJECTS) $(LIBS)

%$(OBJEXT):	%.cpp
	@echo "    Compiling $< ..."
#	@echo "$(CXX) $(CXXFLAGS) $(DEFS) $(CPPFLAGS) $(OUTPUTOBJ)$@ -c $<"
	@$(CXX) $(CXXFLAGS) $(DEFS) $(CPPFLAGS) $(OUTPUTOBJ)$@ -c $<

%$(OBJEXT):	%.c
	@echo "    Compiling $< ..."
#	@echo "$(CC) $(CFLAGS) $(DEFS) $(CPPFLAGS) $(OUTPUTOBJ) $@ -c $<"
	@$(CC) $(CFLAGS) $(DEFS) $(CPPFLAGS) $(OUTPUTOBJ)$@ -c $<

# Clean
#----------------------------------------------------------
clean :
	@echo "    Remove Executables ..."
	@rm -f $(BINNAME)
	@rm -f $(LIBNAME)
	@echo "    Remove Objects ..."
	@rm -f src/*$(OBJEXT)
	@echo "    Remove Generated Files ..."
	@rm -f src/icons.cpp
	@rm -f include/icons.h
	@rm -f include/version.h
#----------------------------------------------------------
















