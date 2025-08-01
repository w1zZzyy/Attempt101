.PHONY: clean build-debug build-release

build-impl-%:
	@mkdir -p build_$* && cd build_$* && \
	cmake -DCMAKE_BUILD_TYPE=$* .. && \
	cmake --build .

build-debug: build-impl-debug
build-release: build-impl-release


clean-impl-%:
	@rm -rf build_$*

clean:
	@${MAKE} clean-impl-debug
	@${MAKE} clean-impl-release