CXXFLAGS += -std=c++11
CXXFLAGS += -I./
CXXFLAGS += -std=c++11 -Wall -Werror -g -c -o

LIB_FILES :=-lglog -lgflags -levent  -lpthread -lssl -lcrypto -lz -lprotobuf -lpthread -ldl

TEST_LIB_FILES :=  -L/usr/local/lib -lgtest -lgtest_main -lpthread

PROTOC = protoc
GRPC_CPP_PLUGIN=grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PROTOS_PATH = ./protos

CPP_SOURCES := \
	./protos/common.pb.cc \
	./protos/scheduler.pb.cc \

CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)


TESTS := \


all: $(CPP_OBJECTS) $(TESTS)
.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

vpath %.proto $(PROTOS_PATH)

.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	@echo "  [GEN]  $@"
	@$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(PROTOS_PATH) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	@echo "  [GEN]  $@"
	@$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(PROTOS_PATH) $<


## /////////////////////////////

clean:
	@rm -fr $(TESTS)
	@echo "rm *_unittest"
	@rm -fr $(CPP_OBJECTS)
	@echo "rm *.o"
