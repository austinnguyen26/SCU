# Lab 5 Makefile

STD=-std=c++11
OPT=-g
TMPMAKE=tmpMake
CXXFLAGS=${STD} ${OPT}
CFLAGS=${OPT}

%.o: %.cxx
	$(CXX) -c $(CXXFLAGS) $<

%: %.cxx
	$(CXX) -o $@ $(CXXFLAGS) $<

ROSTERSRCS=mystring.cxx tester.cxx
ROSTEROBJS=$(ROSTERSRCS:.cxx=.o)
ALL=stringtest

all: $(ALL)

stringtest:	$(ROSTEROBJS)
	$(CXX) -o $@ $(CXXFLAGS) $+

clean:
	/bin/rm -f $(ROSTEROBJS) $(ALL)
	/bin/rm -f roster.tar
	/bin/rm -rf $(ALL:=.dSYM)

# tar:
# 	tar cf roster.tar Makefile roster.cxx roster.h rostermain.cxx

depend: $(ROSTERSRCS)
	/bin/rm -f $(TMPMAKE)
	sed -e '/^# DEPENDENTS/,$$d' Makefile > $(TMPMAKE)
	echo '# DEPENDENTS' >> $(TMPMAKE)
	$(CXX) -MM $+ >> $(TMPMAKE)
	/bin/mv -f $(TMPMAKE) Makefile


# DEPENDENTS
mystring.o: mystring.cxx mystring.h tester.cxx
tester.o: tester.cxx mystring.h mystring.cxx
