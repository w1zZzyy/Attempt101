.PHONY: clear 

build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$* 

test-impl-%: build-impl-%
	@cd build_$*/src/core/tests && ctest

run-impl-%: build-impl-%
	@./build_$*/src/main

build-debug: build-impl-debug
build-release: build-impl-release
test-debug: test-impl-debug 
test-release: test-impl-release
run-debug: run-impl-debug
run-release: run-impl-release

clear:
	@rm -rf build* .cache/