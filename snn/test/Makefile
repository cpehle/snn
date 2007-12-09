## - - - - - - - - - - - - - - - - - - - -

#CXXFLAGS_STANDARD = -g -Wall -pedantic
#CXXFLAGS_LOCAL = -I. -I/home/brett/src/include
#CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_LOCAL}
#PNGFLAGS = `freetype-config --cflags` -I/usr/include -L/usr/lib -lpng -lpngwriter -lz -lfreetype
#OPENCVFLAGS = `pkg-config --libs opencv` `pkg-config --cflags opencv`
LOCAL = -I../include/

BINARIES = main
DEPS = main.cxx ../include/snn.cxx ../include/cfgfile.cxx ../include/snnliaf.cxx ../include/snnliafdt.cxx ../include/snnaeif.cxx

## - - - - - - - - - - - - - - - - - - - -

all: ${BINARIES}

main: ${DEPS}
	${CXX} ${CXXFLAGS} -g ${DEPS} -o ${@} ${OPENCVFLAGS} ${LOCAL}

clean:
	-rm -f ${BINARIES} ${OBJ_FILES}
