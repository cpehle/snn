## - - - - - - - - - - - - - - - - - - - -

#CXXFLAGS_STANDARD = -g -Wall -pedantic
#CXXFLAGS_LOCAL = -I. -I/home/brett/src/include
#CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_LOCAL}
#PNGFLAGS = `freetype-config --cflags` -I/usr/include -L/usr/lib -lpng -lpngwriter -lz -lfreetype
#OPENCVFLAGS = `pkg-config --libs opencv` `pkg-config --cflags opencv`
LOCAL = -I../include/

BINARIES = main
DEPS = main.cxx ../include/cfgfile.cxx ../include/snn/snn.cxx ../include/snn/snnliaf.cxx ../include/snn/snnliafdt.cxx ../include/snn/snnaeif.cxx ../include/snn/snnsconn.cxx ../include/snn/snnaconn.cxx ../include/snn/snnstdpconn.cxx

## - - - - - - - - - - - - - - - - - - - -

all: ${BINARIES}

main: ${DEPS}
	${CXX} ${CXXFLAGS} -g ${DEPS} -o ${@} ${OPENCVFLAGS} ${LOCAL}

clean:
	-rm -f ${BINARIES} ${OBJ_FILES}
