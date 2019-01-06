BUILD_DIR = bld/
SOURCE_DIR = src/
INCLUDE_DIR = inc/
PROG_DIR = prog/

CPPFLAGS += -Iinc

raw-data-db-agent_LDFLAGS = -lzmq
raw-data-db-agent_OBJS = main.o

$(PROG_DIR)raw-data-db-agent: Makefile $(addprefix $(BUILD_DIR), $(raw-data-db-agent_OBJS)) | $(PROG_DIR)
	$(LINK.cpp) -g -o $@ $(filter %.o, $^) $(raw-data-db-agent_LDFLAGS)

sync-source:
	rsync -arvz -e "ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" --exclude=bld --exclude=prog --progress . hc2:~/raw-data-db-agent

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(PROG_DIR):
	mkdir $(PROG_DIR)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp $(INCLUDE_DIR)%.hpp | $(BUILD_DIR)
	$(COMPILE.cpp) -g $< -o $@

clean:
	rm -r $(PROG_DIR)
	rm -r $(BUILD_DIR)
