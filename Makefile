docker-build-desktop-impl-%:
	@docker build -f desktop/Dockerfile -t chessui .
	@${MAKE} -C desktop build-$*

docker-clean:
	@docker system prune -a --volumes

docker-build-desktop-debug: docker-build-desktop-impl-debug
docker-build-desktop-release: docker-build-desktop-impl-release