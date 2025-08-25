.PHONY: build-release build-debug clean test-logic test-logic-debug build-impl-% run-test-impl-%

build-impl-%: 
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. 
	@cmake --build build_$* 

clean:
	@rm -rf build_release build_debug

test-impl-%: build-impl-%
	@./build_$*/tests/tests_exe 

test-release: test-impl-release
test-debug: test-impl-debug

build-release: build-impl-release
build-debug: build-impl-debug

run-engine-release: build-release
	@./build/engine/Engine_exe
run-engine-debug: build-debug 
	@./build/engine/Engine_exe
