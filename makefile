# Generated with github.com/da0x/mmpp
# Binary:
indexify: .obj .obj/main.cpp.o
	g++ -std=c++20 .obj/main.cpp.o -o indexify

.obj:
	mkdir .obj

.obj/main.cpp.o: main.cpp
	g++ -std=c++20 -o .obj/main.cpp.o -c main.cpp

run: indexify
	./indexify

clean:
	rm -rfv .obj

install: indexify
	sudo cp -v indexify /usr/local/bin/

uninstall:
	sudo rm -v /usr/local/bin/indexify
