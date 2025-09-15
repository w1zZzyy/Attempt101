.PHONY: clean-soft clean-hard install-linux-deps rm-linux-deps

build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$* 

test-impl-%: build-impl-%
	@cd build_$*/libs/tests && ctest

run-desktop-impl-%: build-%
	@./build_$*/desktop/AttemptDesktop

install-linux-deps: 
	@chmod +x ./desktop/sfml-deps/install.sh
	@./desktop/sfml-deps/install.sh
rm-linux-deps:
	@chmod +x ./desktop/sfml-deps/remove.sh
	@./desktop/sfml-deps/remove.sh

clean-soft:
	@rm -rf build_debug
	@rm -rf build_release
clean-hard: clean-soft rm-linux-deps

build-debug: build-impl-debug
build-release: build-impl-release
test-debug: test-impl-debug 
test-release: test-impl-release
run-debug: run-desktop-impl-debug 
run-release: run-desktop-impl-release 