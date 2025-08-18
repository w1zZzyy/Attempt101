.PHONY: build-release build-debug clean test-logic test-logic-debug build-impl-% run-test-impl-%

build-impl-%: 
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* ..
	@cmake --build build_$*

clean:
	@rm -rf build_release build_debug

run-test-impl-%: 
	@./build_$*/$*/tests_exe

test-logic: build-impl-release
	@$(MAKE) run-test-impl-logic
test-logic-debug: build-impl-debug
	@$(MAKE) run-test-impl-logic

build-release: build-impl-release
build-debug: build-impl-debug

run-engine-release: build-release
	@./build/engine/Engine_exe
run-engine-debug: build-debug 
	@./build/engine/Engine_exe
