.PHONY: clean

build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$*

build-debug: build-impl-debug
build-release: build-impl-release

docker-build-desktop:
	@git submodule update --init
	@docker build -f desktop/Dockerfile -t chessui .

docker-run-desktop: docker-build-desktop
	@docker run -it -v $(PWD):/app chessui

docker-clean:
	@docker system prune -a --volumes

clean:
	@rm -rf build_debug
	@rm -rf build_release