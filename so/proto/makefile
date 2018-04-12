proto_files = $(wildcard *.proto)

all:
	@echo -e "\033[1;33m\nCreate pb file\033[0m..."
	protoc $(proto_files) --cpp_out=./

clean:
	rm -f *.h *.cc *.o
