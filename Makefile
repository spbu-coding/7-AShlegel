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

.PHONY: clean $(BUILD_DIR) all

all: $(BUILD_DIR) $(EXEC) check

$(OBJECTS): $(SOURCES)
	$(CC) -c $(CFLAGS) -o $@ $^

$(EXEC): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf build/

#--------------------------------------------------------------------------------------

check: $(EXEC)
	test_check=0 ; \
	for test in $(TESTS_NAMES) ; do \
	  ./$< $(TEST_DIR)/$$test.in > $(BUILD_DIR)/$$test.out ; \
	  if cmp -s $(BUILD_DIR)/$$test.out $(TESTS_DIR)/$$test.out ;  then \
	  	echo test $$test passed ; \
	  else \
	    echo test $$test failed ; \
	    test_check=1 ; \
	  fi \
	done ;\
	exit $$test_check ; \

