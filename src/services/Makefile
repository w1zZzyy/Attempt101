CMAKE_COMMON_FLAGS ?= -DUSERVER_OPEN_SOURCE_BUILD=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_DEBUG_FLAGS ?= -DUSERVER_SANITIZE='addr ub'
CMAKE_RELEASE_FLAGS ?=
CMAKE_OS_FLAGS ?= -DUSERVER_FEATURE_CRYPTOPP_BLAKE2=0 -DUSERVER_FEATURE_REDIS_HI_MALLOC=1
NPROCS ?= $(shell nproc)



SERVICE ?=

ifndef SERVICE
$(error You must specify SERVICE)
endif



build_debug/Makefile:
	@cd ${SERVICE}-service && \
	mkdir -p build_debug && \
	cd build_debug && \
	cmake -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS) $(CMAKE_DEBUG_FLAGS) $(CMAKE_OS_FLAGS) $(CMAKE_OPTIONS) ..


build_release/Makefile:
	@cd ${SERVICE}-service && \
	mkdir -p build_release && \
	cd build_release && \
	cmake -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS) $(CMAKE_RELEASE_FLAGS) $(CMAKE_OS_FLAGS) $(CMAKE_OPTIONS) ..

build-impl-%: build_%/Makefile
	@cd ${SERVICE}-service && cmake --build build_$* -j $(NPROCS) --target ${SERVICE}

test-impl-%: build-impl-%
	/usr/local/bin/run_as_user.sh /app/${SERVICE}-service/build_$*/runtests-${SERVICE} --service-logs-pretty -vv

clean-impl-%:
	@cd ${SERVICE}-service/build_$* && ${MAKE} clean 

install-impl-%: build-impl-%
	@cd ${SERVICE}-service/build_$* && cmake --install . -v --component ${SERVICE}



build-debug: build-impl-debug
build-release: build-impl-release
test-debug: test-impl-debug
test-release: test-impl-release
clean-debug: clean-impl-debug 
clean-release: clean-impl-release
install-debug: install-impl-debug 
install-release: install-impl-release



COMPOSE_FILE := ./.devcontainer/docker-compose.yml

docker-impl-%: 
	docker compose -f ${COMPOSE_FILE} run --rm ${SERVICE}-service make $* SERVICE=${SERVICE}

--debug-start-in-docker-%: install-%
	@sed -i 's/config_vars.yaml/config_vars.docker.yaml/g' /app/${SERVICE}-service/configs/static_config.yaml
	@psql 'postgresql://user:password@postgres:5432/chess_db' -f ./postgresql/schemas/db.sql
	@/app/${SERVICE}-service/build_$*/${SERVICE} \
  		--config /app/${SERVICE}-service/configs/static_config.yaml \
		--config_vars /app/${SERVICE}-service/configs/config_vars.yaml



docker-test-debug: docker-impl-test-debug 
docker-test-release: docker-impl-test-release 
docker-start-service-debug:
	@docker compose -f ${COMPOSE_FILE} run --rm ${SERVICE}-service make -- --debug-start-in-docker-debug SERVICE=${SERVICE}
docker-start-service-release:
	@docker compose -f ${COMPOSE_FILE} run --rm ${SERVICE}-service make -- --debug-start-in-docker-release SERVICE=${SERVICE}