all: shared/libCleaner.so shared/libexcept.so shared/libZipper.so shared/libConfig.so
##### Cleaner.so
shared/libCleaner.so: bin/Cleaner-fpic.o
	g++ -g -shared bin/Cleaner-fpic.o -o libCleaner.so -I.
	mv libCleaner.so shared/
#
bin/Cleaner-fpic.o: source/Cleaner.cpp
	g++ -fPIC -c -g source/Cleaner.cpp -o Cleaner-fpic.o -I.
	mv Cleaner-fpic.o bin/
##### except.so
shared/libexcept.so: bin/except-fpic.o
	g++ -g -shared bin/except-fpic.o -o libexcept.so -I.
	mv libexcept.so shared/
#
bin/except-fpic.o: source/exception.cpp
	g++ -fPIC -c -g source/exception.cpp -o except-fpic.o -I.
	mv except-fpic.o bin/
#### Zipper.so
shared/libZipper.so: bin/Zipper-fpic.o
	g++ -g -shared  bin/Zipper-fpic.o -o libZipper.so -Lshared/ -lexcept -lCleaner -Wl,-rpath,shared/ -I.
	mv libZipper.so shared/
#
bin/Zipper-fpic.o: source/Zipper.cpp
	g++ -c -fPIC -g source/Zipper.cpp -o Zipper-fpic.o -I.
	mv Zipper-fpic.o bin/
#### Config.so
shared/libConfig.so: bin/Config-fpic.o
	g++ -g -shared  bin/Config-fpic.o -o libConfig.so -Lshared/ -lZipper -lexcept -lCleaner -Wl,-rpath,shared/ -I.
	mv libConfig.so shared/
#
bin/Config-fpic.o: source/Config.cpp
	g++ -c -fPIC -g source/Config.cpp -o Config-fpic.o -I.
	mv Config-fpic.o bin/
#### parser.c
shared/libparser.so: bin/parser-fpic.o
	g++ -g -shared  bin/parser-fpic.o -o libparser.so -Lshared/ -lZipper -lexcept -lCleaner -Wl,-rpath,shared/ -I.
	mv libparser.so shared/
#
bin/parser-fpic.o: source/parser.cpp
	g++ -c -fPIC -g source/parser.cpp -o parser-fpic.o -I.
	mv parser-fpic.o bin/
##################################
clean: clean-so clean-o clean-a
clean-so:
	rm -rf shared/*.so
clean-a:
	rm -rf lib/*.a
clean-o:
	rm -rf bin/*.o
