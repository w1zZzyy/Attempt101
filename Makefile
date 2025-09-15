.PHONY: clean docker-build-debug docker-build-release

build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$* 

test-impl-%: build-impl-%
	@cd build && ctest

run-desktop-impl-%: docker-build-%
	@./build_$*/desktop/AttemptDesktop

build-debug: build-impl-debug
build-release: build-impl-release
test-debug: test-impl-debug 
test-release: test-impl-release
run-debug: run-desktop-impl-debug 
run-release: run-desktop-impl-release 

clean:
	@rm -rf build_debug
	@rm -rf build_release