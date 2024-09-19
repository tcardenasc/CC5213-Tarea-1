# CC5213 - Recuperación de Información Multimedia
# Profesor: Juan Manuel Barrios
# Fecha: 07 de agosto de 2024

# parametros para compilación: usar C++ 11 y con todos los warnings
CFLAGS += -std=c++17 -Wall -Wextra

#compilar con optimizaciones. Para debug poner -O0 -ggdb
CFLAGS += -O3

#usar opencv para compilación y linkeo
CFLAGS  += $(shell pkg-config --cflags opencv4 Qt5Widgets)
LDFLAGS += $(shell pkg-config --libs   opencv4 Qt5Widgets)

##si no funciona pkg-config se deben configurar las rutas de instalacion de opencv
#CFLAGS  += -I[ruta al opencv/include]
#LDFLAGS += -L[ruta al opencv/lib] -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

#otras librerías que se podrían necesitar
#LDFLAGS  += -lm -lpthread

########## archivos a compilar ##########

MAIN_CPP        := $(sort $(wildcard tarea1*.cpp))
HELPERS_CPP     := $(wildcard helper*.cpp)
HELPERS_HEADERS := $(wildcard helper*.hpp)

########## archivos a generar ##########

MAIN_BIN  := $(patsubst %,bin/%,$(basename $(MAIN_CPP)))
HELPERS_O := $(patsubst %,build/%,$(HELPERS_CPP:.cpp=.o))

########## reglas de compilación ##########

#reglas all y clean no corresponden a archivos
.PHONY: all clean

#no borrar archivos intermedios
.PRECIOUS: build/%.o

#por defecto se generan todos los ejecutables de los ejemplos
all: $(MAIN_BIN)

#para cada ejecutable se requiere el object correspondiente más los helpers
bin/%: build/%.o $(HELPERS_HEADERS) $(HELPERS_O)
	mkdir -p "$(@D)"
	g++ $< -o $@ $(HELPERS_O) $(LDFLAGS)

#para generar un object se usa el fuente cpp correspondiente más los headers
build/%.o: %.cpp $(HELPERS_HEADERS)
	mkdir -p "$(@D)"
	g++ -c $(CFLAGS) -o $@ $<

#limpiar todos los archivos de compilación
clean:
	rm -rf bin/ build/
