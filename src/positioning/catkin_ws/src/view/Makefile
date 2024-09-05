
all: parking_map.c resource.c robo_parking_map.c robo_resource.c
	gcc -o show_parkingmap parking_map.c resource.c `pkg-config --libs --cflags gtk+-3.0` -rdynamic -lm
	gcc -o robo_show_parkingmap robo_parking_map.c robo_resource.c `pkg-config --libs --cflags gtk+-3.0` -rdynamic -lm
	gcc -o robo_show_parkingmap_devel robo_parking_map_devel.c robo_resource.c `pkg-config --libs --cflags gtk+-3.0` -rdynamic -lm


resource.c: resource.xml
	glib-compile-resources --target=resource.c --generate-source resource.xml

robo_resource.c: robo_resource.xml
	glib-compile-resources --target=robo_resource.c --generate-source robo_resource.xml