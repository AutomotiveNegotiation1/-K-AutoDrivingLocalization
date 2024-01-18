
all: parking_map.c resource.c 
	gcc -o show_parkingmap parking_map.c resource.c `pkg-config --libs --cflags gtk+-3.0` -rdynamic -lm


resource.c: resource.xml
	glib-compile-resources --target=resource.c --generate-source resource.xml
