
EXEC := buscaminas

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -Wall  `pkg-config --cflags CEGUI` `pkg-config --cflags OGRE` $(CEGUI_CFLAGS) `pkg-config --cflags OIS`

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs CEGUI` `pkg-config --libs CEGUI-OGRE` `pkg-config --libs OGRE` -lOIS -lGL -lstdc++ $(CEGUI_LDFLAGS) -lboost_system -lSDL -lSDL_mixer

# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O2 -D_RELEASE
else 
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: dirs info $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

dirs:
	mkdir -p $(DIROBJ)
# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

# Compilación --------------------------------------------------------
$(DIROBJ)main.o: $(DIRSRC)main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
$(DIROBJ)%.o: $(DIRSRC)%.cpp $(DIRHEA)%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~ 
