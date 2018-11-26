# ****************************
# **** FABIO  DE LORENZO *****
# **** fabio@crokky.com  *****
# ****************************
# ****************************

# enalbe GDB on for debugging
GDB = -g -O0 -fPIC -Wall
 LIBRARIES=-L../librtmp -lrtmp -lm

all: RtmpAacExtractor.cpp
	g++ $(GDB) -o RtmpAacExtractor.exe FlashVideo.cpp RtmpWrapper.cpp RtmpAacExtractor.cpp $(LIBRARIES)

parseflv:
	gcc $(GDB) -o parseflv.exe functions.c libflv.c parseflv.c $(LIBRARIES)
clean:
	$(RM) *.exe *.o

install:
	cp *.so ../libs/
