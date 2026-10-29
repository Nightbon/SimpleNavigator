.PHONY: all clean style gcov_report check valgrind cppcheck test s21_graph s21_graph_algorithms

BUILD_DIR = ../build
BUILD_COV_DIR = ../build_coverage
CMAKE = cmake
PREFIX ?= $(BUILD_DIR)
INSTALL_DIR = $(PREFIX)/SimpleNavigator

all:	$(BUILD_DIR)/Makefile
	$(CMAKE) --build $(BUILD_DIR) --target s21_graph s21_graph_algorithms simpleNavigator

test:	$(BUILD_DIR)/Makefile
	$(CMAKE) --build $(BUILD_DIR) --target test_model && ./$(BUILD_DIR)/test_model

s21_graph: $(BUILD_DIR)/Makefile
	$(CMAKE) --build $(BUILD_DIR) --target s21_graph

s21_graph_algorithms: $(BUILD_DIR)/Makefile
	$(CMAKE) --build $(BUILD_DIR) --target s21_graph_algorithms

$(BUILD_DIR)/Makefile:
	mkdir -p $(BUILD_DIR)
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

gcov_report: $(BUILD_COV_DIR)/Makefile
	$(CMAKE) --build $(BUILD_COV_DIR) --target test_model
	
	./$(BUILD_COV_DIR)/test_model
	
	mkdir -p $(BUILD_COV_DIR)/report
	
	gcovr -r . \
		--gcov-executable "gcov" \
		--html --html-details -o $(BUILD_COV_DIR)/report/index.html \
		--print-summary \
		--exclude ".*test_*" \
		--exclude-throw-branches \
		--exclude-unreachable-branches \
		$(BUILD_COV_DIR)
	
	@echo "Отчет успешно сгенерирован: $(BUILD_COV_DIR)/report/index.html"

$(BUILD_COV_DIR)/Makefile:
	mkdir -p $(BUILD_COV_DIR)
	$(CMAKE) -S . -B $(BUILD_COV_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS="--coverage -g" \
		-DCMAKE_EXE_LINKER_FLAGS="--coverage"

clean:
	rm -rf $(BUILD_DIR) $(BUILD_COV_DIR) ../dist
	
dist:
	mkdir -p ../dist
	cp -r ../src ../dist/
	cd ../dist && tar -czf ../dist/SimpleNavigator.tar.gz src
	rm -rf ../dist/src

style:
	find . \( -name "*.cpp" -o -name "*.h" \) | xargs clang-format -n --style=Google

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=missingInclude --suppress=unusedFunction --suppress=unusedStructMember model/

valgrind: test
	cd $(BUILD_DIR) && valgrind --leak-check=full ./test_model

check: clean gcov_report style cppcheck valgrind
	@echo "All checks passed!"