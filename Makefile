CXX = g++

IDIR=./include

CXXFLAGS = `root-config --cflags --libs` -Wno-unused-command-line-argument -std=c++17 -I$(IDIR)

TARGET = ecorsika

ODIR=./obj

_DEPS= io.hh DBReader.hh
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ= ecorsika.o io.o DBReader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


all: $(TARGET)


$(ODIR)/%.o: src/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS)


$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@


.PHONY: clean

clean:
	rm -f $(TARGET) src/*~ $(ODIR)/*.o $(IDIR)/*~  *~ core 
