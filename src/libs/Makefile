.PHONY: build-release build-debug clean test-logic test-logic-debug build-impl-% run-test-impl-%

build-impl-%: 
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=$* ..
	@cmake --build build

clean:
	@rm -rf build

run-test-impl-%: 
	@./build/$*/tests_exe

test-logic: build-impl-Release
	@$(MAKE) run-test-impl-logic
test-logic-debug: build-impl-Debug
	@$(MAKE) run-test-impl-logic

build-release: build-impl-Release
build-debug: build-impl-Debug

run-engine-release: build-release
	@./build/engine/Engine_exe
run-engine-debug: build-debug 
	@./build/engine/Engine_exe
