COMM_DIR :=../comm
PROTOBUFPATH:=../../protobuf-2.4.1/src/
INC :=  -I ./ -I$(COMM_DIR)   -I./hcomm -I$(PROTOBUFPATH) -I/usr/local/spp/module/include/spp_incl/ -I/usr/local/spp/module/include -I/usr/local/spp/module/include/sync_incl/ -I../3rd/txtprotobuf/ 
LIBDIRS =
LIB :=  $(PROTOBUFPATH)/../lib/libprotobuf.a ./hcomm/libhcomm.a  -lpthread  -lstdc++ -lc  


SHAREDLIB =
WARNINGS := -Wall -Wcast-qual -Wsign-compare
MACROS := -DMFP_EPOLL -DSYSEPOLL  -D_LARGEFILE_SOURCE
CFLAGS= -g -fPIC -shared  -pipe -fno-ident -MMD -D_GNU_SOURCE -D_REENTRANT ${WARNINGS}  ${MACROS}  

CC=g++ $(CFLAGS)

SOURCESCC = $(wildcard *.cc)
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
PBOBJ= $(patsubst %.proto,release/%.pb.o,$(wildcard *.proto))

RELOBJFILESCC = $(SOURCESCC:%.cc=release/%.o)
RELOBJFILES = $(SOURCES:%.cpp=release/%.o)

RELOBJFILESCC += $(PBOBJ)


.PHONY: pbfiles all

TARGET= librpcframe.a 

all: $(TARGET)

pbfiles:
	./compile_proto.sh	

$(RELOBJFILES): release/%.o: %.cpp $(HEADERS)
	@mkdir -p release
	$(CC) -c $< -o $@ $(INC)

$(RELOBJFILESCC): release/%.o: %.cc $(HEADERS)
	@mkdir -p release
	$(CC) -c $< -o $@ $(INC)

$(TARGET): $(RELOBJFILES) $(RELOBJFILESCC)
	ar q $@ $^
	#$(CC) -o $@ $^ $(CFLAGS) $(LIB)

clean:
	rm -rf release ${TARGET}
#	rm *.pb.cc
#	rm *.pb.h
