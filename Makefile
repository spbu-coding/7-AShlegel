include CONFIG.cfg

CC = gcc
LD = gcc
CFLAGS = -g -O2 -Wall -Wextra -Wpedantic -Werror
LDFLAGS =

EXEC = $(BUILD_DIR)/$(NAME)
SOURCES = $(SOURCE_DIR)/$(NAME).c
OBJECTS = $(EXEC).o

TESTS_IN = $(sort $(wildcard $(TEST_DIR)/*.in))
TESTS_NAMES = $(TESTS_IN:$(TEST_DIR)/%.in=%)
TESTS_OUT = $(sort $(wildcard $(TEST_DIR)/*.out))
SORT_OUT = $(TESTS_OUT:$(TEST_DIR)/%=$(BUILD_DIR)/%)

.PHONY: clean all check

all: $(EXEC)

$(OBJECTS): $(SOURCES) | $(BUILD_DIR)

	$(CC) -c $(CFLAGS) -o $@ $<

$(EXEC): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $<

$(BUILD_DIR): $(SOURCES)
	mkdir -p $@

clean:
	rm -rf build/

check: $(BUILD_DIR)/log
	@exit $$(cat $<)

$(BUILD_DIR)/log: $(EXEC) $(TESTS_IN) $(TESTS_OUT)
	@test_check=0 ; \
	for test in $(TESTS_NAMES) ; do \
	  ./$< $(TEST_DIR)/$$test.in > $(BUILD_DIR)/$$test.out ; \
	  if cmp -s $(BUILD_DIR)/$$test.out $(TEST_DIR)/$$test.out ;  then \
	  	echo test $$test passed ; \
	  else \
	    echo test $$test failed ; \
	    test_check=1 ; \
	  fi \
	done ;\
	echo $$test_check > $@
