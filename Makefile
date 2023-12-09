.PHONY: all server user tests

all: server user tests

server:
	$(MAKE) -C src/server

user:
	$(MAKE) -C src/user

tests:
	$(MAKE) -C tests/our

clean:
	$(MAKE) -C src/server clean
	$(MAKE) -C src/user clean
	$(MAKE) -C tests/our clean
