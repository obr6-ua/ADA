# Nombre del archivo del compilador
CXX=g++

# Opciones de compilación, -O3 para optimización y -std=c++11 para asegurar el uso de C++11
CXXFLAGS=-std=c++11 -O3

# Nombres de los ejecutables
TARGET1=pracFinal
TARGET2=pracFinalOptimizada

# Objeto archivos que necesita para los ejecutables finales
OBJS1=pracFinal.o
OBJS2=pracFinalOptimizada.o

# Regla por defecto
all: $(TARGET1) $(TARGET2)

# Como construir el target
$(TARGET1): $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(OBJS1)

$(TARGET2): $(OBJS2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(OBJS2)

# Como construir los .o de los .cc
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

# Limpiar los archivos intermedios
clean:
	rm -f $(OBJS1) $(OBJS2) $(TARGET1) $(TARGET2)

# Regla para empezar de nuevo
distclean: clean
	rm -f $(TARGET1) $(TARGET2)
