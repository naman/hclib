include $(HCLIB_ROOT)/include/hclib.mak

EXE=FFT fib nqueens qsort fib-ddt Cilksort

all: clean $(EXE) clean-obj

FFT: FFT.cpp
	$(CXX) $(PROJECT_CXXFLAGS) $(PROJECT_LDFLAGS) -o $@ $^ $(PROJECT_LDLIBS)

Cilksort: Cilksort.cpp
	$(CXX) $(PROJECT_CXXFLAGS) $(PROJECT_LDFLAGS) -o $@ $^ $(PROJECT_LDLIBS)

nqueens: nqueens.cpp
	$(CXX) $(PROJECT_CXXFLAGS) $(PROJECT_LDFLAGS) -o $@ $^ $(PROJECT_LDLIBS)

qsort: qsort.cpp
	$(CXX) $(PROJECT_CXXFLAGS) $(PROJECT_LDFLAGS) -o $@ $^ $(PROJECT_LDLIBS)

fib-ddt: fib-ddt.cpp
	$(CXX) $(PROJECT_CXXFLAGS) $(PROJECT_LDFLAGS) -o $@ $^ $(PROJECT_LDLIBS)

fib: fib.cpp
	$(CXX) $(PROJECT_CXXFLAGS) -o $@ $^ -L . $(PROJECT_LDLIBS) $(PROJECT_LDFLAGS)

clean-obj:
	rm -rf *.o *.dSYM

clean:
	rm -rf *.o $(EXE) *.dSYM
