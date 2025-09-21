build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$* 

test-impl-%: build-impl-%
	@cd build_$*/core/tests && ctest

build-debug: build-impl-debug
build-release: build-impl-release
test-debug: test-impl-debug 
test-release: test-impl-release
