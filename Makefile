.PHONY: clean docker-build-debug docker-build-release

build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$*

build-debug: build-impl-debug
build-release: build-impl-release


run-desktop-impl-%:
	@./build_$*/desktop/ChessDesktop

run-desktop-debug: run-desktop-impl-debug 
run-desktop-release: run-desktop-impl-release 
	

docker-build-impl-%:
	@git submodule update --init
	@docker build -f desktop/Dockerfile -t chessui .
	@docker run --rm \
	-v $(PWD):${PWD} \
	-w ${PWD} \
	chessui ${MAKE} build-$*

docker-build-debug: docker-build-impl-debug 
docker-build-release: docker-build-impl-release

docker-clean:
	@docker system prune -a --volumes

clean:
	@rm -rf build_debug
	@rm -rf build_release