
BIN := bin
SCANNER := $(BIN)/lex.yy.c
PARSER  := $(BIN)/sprokit.tab.cc
COMPILER := $(BIN)/sprokit
INCLUDE := -Ibin/ -Isrc/parser -Isrc/ast -Isrc/adt -Isrc/semantics -Isrc/codegen

SRCS := $(shell find src/ -name \*.cpp)
OBJS := $(addsuffix .o,$(addprefix bin/,$(basename $(SRCS))))
DEPS := $(OBJS:%.o=%.d)

CPP := g++
CPPARGS := -std=c++23 -Wall -g $(INCLUDE)

.PHONY: all scanner parser test clean realclean

all: test

bin/%.o: %.cpp $(SCANNER)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPARGS) -MMD -o $@ -c $<

-include $(DEPS)

$(COMPILER): $(SCANNER) $(PARSER) $(OBJS)
	$(CPP) $(CPPARGS) -o $@ $^

scanner: $(SCANNER)

parser: $(PARSER)

$(SCANNER): src/scanner/sprokit.l $(PARSER)
	@mkdir -p $(dir $@)
	flex -o $@ $<

$(PARSER): src/parser/sprokit.y
	@mkdir -p $(dir $@)
	bison -Wcounterexamples -d -v -o $@ $<

clean:
	rm $(SCANNER)
	rm $(PARSER)
	rm $(COMPILER)
	rm $(OBJS)

realclean:
	rm -rf $(BIN)

test: $(COMPILER)
	./test/test.sh
