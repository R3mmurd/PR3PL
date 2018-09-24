# Adapta esto a tu compilador
CXX = clang++ -std=c++14

# Adapta esta ruta a tu instalación de DeSIGNAR
DSG = ~/DeSIGNAR

# De aquí en adelante, es preferible que no toques estas variables
WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wno-invalid-source-encoding

FLAGS = -DDEBUG -O0 -g $(WARN)

OPT = -DNDEBUG -Ofast -D__extern_always_inline="extern __always_inline" -DNDEBUG -std=c++14 $(WARN)

INCLUDE = -I. -I $(DSG)/include

LIBS = -L $(DSG)/lib -lDesignar -lc -lm -lreadline


all: test interprete

test: expnode.H helpers.H test.C
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

interprete: expnode.H helpers.H interprete.C
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

clean:
	$(RM) test interprete *~
