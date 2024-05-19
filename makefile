# Nombre del archivo del compilador
CXX=g++

# Opciones de compilación, -O3 para optimización y -std=c++11 para asegurar el uso de C++11
CXXFLAGS=-std=c++11 -O3

# Nombre del ejecutable
TARGET=mcp_bt

# Objeto archivos que necesita para el ejecutable final
OBJS=mcp_bt.o

# Regla por defecto
all: $(TARGET)

# Como construir el target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Como construir los .o de los .cc
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

# Limpiar los archivos intermedios
clean:
	rm -f $(OBJS) $(TARGET)

# Regla para empezar de nuevo
distclean: clean
	rm -f $(TARGET)
