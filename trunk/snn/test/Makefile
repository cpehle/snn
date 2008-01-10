## - - - - - - - - - - - - - - - - - - - -

#CXXFLAGS_STANDARD = -g -Wall -pedantic
#CXXFLAGS_LOCAL = -I. -I/home/brett/src/include
#CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_LOCAL}
#PNGFLAGS = `freetype-config --cflags` -I/usr/include -L/usr/lib -lpng -lpngwriter -lz -lfreetype
#OPENCVFLAGS = `pkg-config --libs opencv` `pkg-config --cflags opencv`
LOCAL = -I../include/

BINARIES = main

OBJFILES = ../objectFiles/main.o ../objectFiles/snn.o ../objectFiles/cfgfile.o ../objectFiles/snnliaf.o ../objectFiles/snnliafdt.o ../objectFiles/snnaeif.o ../objectFiles/snnsconn.o ../objectFiles/snnaconn.o ../objectFiles/snnstdpconn.o ../objectFiles/snnbase.o

#SNNDEPS = cfgfile.o snnliaf.o snnliafdt.o snnaeif.o snnsconn.o snnaconn.o snnstdpconn.o
#SNNCXXS = ../include/cfgfile.cxx ../include/snn/snn.cxx ../include/snn/snnliaf.cxx ../include/snn/snnliafdt.cxx ../include/snn/snnaeif.cxx ../include/snn/snnsconn.cxx ../include/snn/snnaconn.cxx ../include/snn/snnstdpconn.cxx..
SNNHEADERS = ../include/cfgfile.h ../include/snn/snnliaf.h ../include/snn/snnliafdt.h ../include/snn/snnaeif.h ../include/snn/snnsconn.h ../include/snn/snnaconn.h ../include/snn/snnstdpconn.h ../include/snn/snnbase.h

DEPS = main.cxx

## - - - - - - - - - - - - - - - - - - - -

all: ${BINARIES}

# main file
main: ${OBJFILES}
	${CXX} ${OBJFILES} -o ${@}
#	-rm -f ${OBJFILES}

../objectFiles/main.o: ../include/snn.h main.cxx
	${CXX} -c main.cxx -o ${@} ${LOCAL}

# configuration file reading
../objectFiles/cfgfile.o: ../include/cfgfile.h ../include/cfgfile.cxx
	${CXX} -c ../include/cfgfile.cxx -o ${@} ${LOCAL}

# snn package
../objectFiles/snn.o: ${SNNHEADERS} ../include/snn/snn.cxx
	${CXX} -c ../include/snn/snn.cxx -o ${@} ${LOCAL}

../objectFiles/snnbase.o: ../include/snn/snnbase.h ../include/snn/snnbase.cxx
	${CXX} -c ../include/snn/snnbase.cxx -o ${@} ${LOCAL}

# ======================

../objectFiles/snnliaf.o: ../include/snn/snnliaf.h ../include/snn/snnliaf.cxx
	${CXX} -c ../include/snn/snnliaf.cxx -o ${@} ${LOCAL}

../objectFiles/snnliafdt.o: ../include/snn/snnliafdt.h ../include/snn/snnliafdt.cxx
	${CXX} -c ../include/snn/snnliafdt.cxx -o ${@} ${LOCAL}

../objectFiles/snnaeif.o: ../include/snn/snnaeif.h ../include/snn/snnaeif.cxx
	${CXX} -c ../include/snn/snnaeif.cxx -o ${@} ${LOCAL}

../objectFiles/snnsconn.o: ../include/snn/snnsconn.h ../include/snn/snnsconn.cxx
	${CXX} -c ../include/snn/snnsconn.cxx -o ${@} ${LOCAL}

../objectFiles/snnaconn.o: ../include/snn/snnaconn.h ../include/snn/snnaconn.cxx
	${CXX} -c ../include/snn/snnaconn.cxx -o ${@} ${LOCAL}

../objectFiles/snnstdpconn.o: ../include/snn/snnstdpconn.h ../include/snn/snnstdpconn.cxx
	${CXX} -c ../include/snn/snnstdpconn.cxx -o ${@} ${LOCAL}

# OLD
#main: ${DEPS}
#	${CXX} ${CXXFLAGS} -g ${DEPS} -o ${@} ${OPENCVFLAGS} ${LOCAL}

clean:
	-rm -f ${BINARIES} ${OBJFILES}
