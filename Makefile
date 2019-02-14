BUILD_DIR = bld/
SOURCE_DIR = src/
INCLUDE_DIR = inc/
PROG_DIR = prog/
TEST_DIR = src_test/

CPPFLAGS += -Iinc

uuid_OBJS = uuid.o
uuid_LDFLAGS = -luuid

test-uuid_OBJS = $(uuid_OBJS) uuid.test.o gtest.o
test-uuid_LDFLAGS = $(uuid_LDFLAGS) -lgtest

repo_OBJS = repo.o $(uuid_OBJS)
repo_LDFLAGS = -lpqxx -lpq $(uuid_LDFLAGS)

raw-data-db-agent_OBJS = main.o $(repo_OBJS)
raw-data-db-agent_LDFLAGS = -lzmq $(repo_LDFLAGS)

$(PROG_DIR)raw-data-db-agent: Makefile $(addprefix $(BUILD_DIR), $(raw-data-db-agent_OBJS)) | $(PROG_DIR)
	$(LINK.cpp) -g -o $@ $(filter %.o, $^) $(raw-data-db-agent_LDFLAGS)

$(PROG_DIR)test-uuid: Makefile $(addprefix $(BUILD_DIR), $(test-uuid_OBJS)) | $(PROG_DIR)
	$(LINK.cpp) -g -o $@ $(filter %.o, $^) $(test-uuid_LDFLAGS)

sync-source:
	rsync -arvz -e "ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" --exclude=.git --exclude=bld --exclude=prog --progress . hc2:~/raw-data-db-agent

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(PROG_DIR):
	mkdir $(PROG_DIR)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp $(INCLUDE_DIR)%.hpp | $(BUILD_DIR)
	$(COMPILE.cpp) -g $< -o $@

$(BUILD_DIR)%.o: $(TEST_DIR)%.cpp | $(BUILD_DIR)
	$(COMPILE.cpp) -g $< -o $@

clean:
	rm -r $(PROG_DIR)
	rm -r $(BUILD_DIR)
