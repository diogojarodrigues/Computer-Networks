.PHONY: all server user

all: server user

server:
	$(MAKE) -C server

user:
	$(MAKE) -C user

clean:
	$(MAKE) -C server clean
	$(MAKE) -C user clean
