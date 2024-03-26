

ifndef COAST_DIR
$(error COAST_DIR is not set)
endif

CXX = g++

IDIR = -I$(COAST_DIR)/include

CXXFLAGS = `root-config --cflags` -fPIC -ggdb3 -Wl,--no-as-needed -std=c++17 $(IDIR)
LDFLAGS = `root-config --libs` -L$(COAST_DIR)/lib -L$(COAST_DIR)/lib/unknown -lCorsikaFileIO -lCorsikaIntern

TARGET = corsika2root 

all: $(TARGET)

$(TARGET): corsika2root.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)


.PHONY: clean

clean:
	rm -f $(TARGET) *~ core 
