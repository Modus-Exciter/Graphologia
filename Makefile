# Универсальный Makefile, автоматически ищущий файлы исходного кода в папке src,
# а файлы .h в папке include. Все зависимости определяются автоматически

# Название программы, которую требуется собрать
EXEC:=Graph.exe

SRC:=src
TST:=test
HDR:=include
RUN:=run

ifndef BUILD
BIN:=bin
else
BIN:=bin/$(BUILD)
endif
OBJ:=obj

SOURCES:=$(wildcard $(SRC)/*.cpp)
OBJS:=$(SOURCES:$(SRC)/%.cpp=$(OBJ)/%.o)

define NEWLINE


endef

DEPEN:=$(shell g++ -MM -I$(HDR) $(SOURCES))
DEPEN:=$(filter %.o: $(HDR)/%.h,$(DEPEN))
DEPEN:=$(patsubst %.o:,$(NEWLINE)$(OBJ)/%.o:, $(DEPEN))

.PHONY: all clean test

ifeq ($(BUILD), Release)
.INTERMEDIATE:  $(OBJS)
endif

all: $(BIN)/$(EXEC)

# Дополнительные зависимости
$(eval $(DEPEN))

$(OBJS): $(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	@echo "Compiling file $<..."
	-g++ -o $@ -Wall -c -I$(HDR) -std=c++11 $< -g

$(BIN)/$(EXEC): $(OBJS) | $(BIN)
	@echo "Linking..."
	g++ -o $@ $(OBJS)
	@echo "Build succeeded!"

$(BIN) $(OBJ) $(TST)\$(RUN):
	mkdir $@

clean:
	-rd /s/q $(BIN)
	-rd /s/q $(OBJ)
	-rd /s/q $(TST)\$(RUN)
	
OBJS2:=$(subst $(OBJ)/main.o,,$(OBJS))
TESTS:=$(wildcard $(TST)/*.cpp)
TOBJS:=$(TESTS:$(TST)/%.cpp=$(TST)/run/%.o)
TEXES:=$(TESTS:$(TST)/%.cpp=$(TST)/run/%.exe)
TRUNS:=$(TESTS:$(TST)/%.cpp=%)

.INTERMEDIATE:  $(TOBJS)

test: $(TRUNS)

$(TRUNS) : % : $(TST)/$(RUN)/%.exe
	@echo "Running $@"
	-@$<
	
$(TEXES) : $(TST)/$(RUN)/%.exe : $(TST)/$(RUN)/%.o $(OBJS2) 
	g++ -o $@ $< $(OBJS2)
	
$(TOBJS) : $(TST)/$(RUN)/%.o : $(TST)/%.cpp | $(TST)\$(RUN)
	-g++ -o $@ -c -I$(HDR) -std=c++11 $< -g