.PHONY: clean

build-impl-%:
	@mkdir -p build_$*
	@cd build_$* && cmake -DCMAKE_BUILD_TYPE=$* .. 
	@cmake --build build_$*

build-debug: build-impl-debug
build-release: build-impl-release


run-desktop-impl-%: build-%
	@./build_$*/desktop/ChessDesktop

run-desktop-debug: run-desktop-impl-debug 
run-desktop-release: run-desktop-impl-release 


docker-build-desktop:
	@git submodule update --init
	@docker build -f desktop/Dockerfile -t chessui .

docker-run-desktop: docker-build-desktop
	@xhost +local:root
	@docker run -it \
	--device /dev/dri \
	-e DISPLAY=$(DISPLAY) \
	-e LIBGL_ALWAYS_SOFTWARE=1 \
	-e SDL_VIDEO_X11_NO_SHM=1 \
	-v /tmp/.X11-unix:/tmp/.X11-unix \
	-v $(PWD):/app \
	chessui
	@xhost -local:root

docker-clean:
	@docker system prune -a --volumes

clean:
	@rm -rf build_debug
	@rm -rf build_release