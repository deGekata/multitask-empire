CC = cxx

CXXFLAGS  = -c
# CXXFLAGS += -fno-elide-constructors
LDFLAGS = # -fsanitize=address

SRCDIRS = ./src/
TARGETDIRS = ./targets/
BINDIR = ./bin/
INCLUDEDIR = ./include/

SOURCES = $(shell find $(SRCDIRS)*.cpp)
TARGETS = $(shell find $(TARGETDIRS)*.cpp)

OBJECTS = $(addprefix $(BINDIR), $(notdir $(SOURCES:.cpp=.o)))
TARGETOBJECTS = $(addprefix $(BINDIR), $(notdir $(TARGETS:.cpp=.o)))

DEPENDENCES = $(addsuffix .d,$(OBJECTS))
TARGETDEPENDENCES = $(addsuffix .d,$(TARGETOBJECTS))

TARGETSNOEXT =  $(notdir $(TARGETS:.cpp=))

$(TARGETSNOEXT): $(OBJECTS) $(TARGETOBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) $(BINDIR)$@.o -o $@.out

$(BINDIR)%.o: $(SRCDIRS)%.cpp
	@$(CC) -MMD -MF $@.d $(CXXFLAGS) $< -o $@

$(BINDIR)%.o: $(TARGETDIRS)%.cpp
	@$(CC) -MMD -MF $@.d $(CXXFLAGS) $< -o $@

-include $(DEPENDENCES)
-include $(TARGETDEPENDENCES)

.PHONY: all, clean, mkdirs

all: Library

mkdirs:
	mkdir $(BINDIR) $(SRCDIRS) $(TARGETDIRS) $(INCLUDEDIR)

clean: 
	rm $(OBJECTS) $(TARGETOBJECTS) $(DEPENDENCES) $(TARGETDEPENDENCES)
