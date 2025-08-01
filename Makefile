build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$*

build-debug: build-impl-debug
build-release: build-impl-release

docker-build-desktop:
	@git submodule update --init
	@docker build -f desktop/Dockerfile -t chessui .

docker-run-desktop-impl-%: docker-build-desktop
	@docker run -it chessui

docker-clean:
	@docker system prune -a --volumes

docker-run-desktop-debug: docker-run-desktop-impl-debug 
docker-run-desktop-release: docker-run-desktop-impl-release