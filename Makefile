CXX = g++

IDIR=./include
LIBDIR=./lib

CXXFLAGS = `root-config --cflags --libs` -Wno-unused-command-line-argument -std=c++17 -I$(IDIR) -fPIC

TARGET = ECosmic
LIBRARY = lib$(TARGET).so

ODIR=./obj

_DEPS= io.hh DBReader.hh distribute.hh detector.hh EHandler.hh EShower.hh EParticle.hh
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJLIB= io.o DBReader.o detector.o EHandler.o EShower.o EParticle.o
OBJLIB = $(patsubst %,$(ODIR)/%,$(_OBJLIB))

_OBJ= ecorsika.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


all: $(TARGET) $(LIBRARY)

lib: $(LIBRARY)

exe: $(TARGET)


$(ODIR)/%.o: src/%.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS)

$(LIBRARY): $(OBJLIB)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(LIBDIR)/$@

$(TARGET): $(OBJ) $(OBJLIB)
	$(CXX) $(CXXFLAGS) $^ -o $@


.PHONY: clean

clean:
	rm -f $(TARGET) src/*~ lib/*.so $(ODIR)/*.o $(IDIR)/*~  *~ core 
