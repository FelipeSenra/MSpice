# Project: Project1
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  = 
OBJ  = temp/LinearAlgebra/algebra.o temp/component.o temp/main.o    $(RES)
LINKOBJ  = temp/LinearAlgebra/algebra.o temp/component.o temp/main.o   $(RES)
LIBS =  -L"C:/Dev-Cpp/lib"  
INCS =  -I"C:/Dev-Cpp/include" 
CXXINCS =  -I"C:/Dev-Cpp/lib/gcc/mingw32/3.4.2/include"  -I"C:/Dev-Cpp/include/c++/3.4.2/backward"  -I"C:/Dev-Cpp/include/c++/3.4.2/mingw32"  -I"C:/Dev-Cpp/include/c++/3.4.2"  -I"C:/Dev-Cpp/include" 
BIN  = spice.exe
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before spice.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "spice.exe" $(LIBS)
	
temp/LinearAlgebra/algebra.o: temp/LinearAlgebra/algebra.c
	$(CPP) -c temp/LinearAlgebra/algebra.c -o temp/LinearAlgebra/algebra.o $(CXXFLAGS)

temp/component.o: temp/component.c
	$(CPP) -c temp/component.c -o temp/component.o $(CXXFLAGS)

temp/main.o: temp/main.c
	$(CPP) -c temp/main.c -o temp/main.o $(CXXFLAGS)




