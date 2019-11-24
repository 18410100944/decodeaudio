CXX			=	g++
TARGET		=	decoderAudio
OBJECTS		=	test.o decoder.o 
LFLAGS      =	-std=c++11 -m64 -o 
LIBS		=	-L ../lib/ -lavcodec -lavformat -lswscale -lavutil -lavfilter -lswresample
INCLUDES	=	-I./ -I../include/ 

$(TARGET):$(OBJECTS)
	$(CXX)  $(LFLAGS) $(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS):%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o  $@ $(INCLUDES) $<

#g++ -g -o test convertaudio.cpp audiocoder.cpp test.cpp -I../include/ -L../lib/ -lspeex -lspeexdsp -lmp3lame -lz

# $(TARGET):
# 	$(CXX) $(LFLAGS) $(TARGET) 
clean:
	rm  -rf $(TARGET) *.o test
.PHONY:clean
