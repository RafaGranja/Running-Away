ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: running_away.exe

running_away.exe: running_away.o
	gcc -o running_away.exe running_away.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

running_away.o: running_away.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c running_away.c		
		
		
clean:
	del running_away.o 
	del running_away.exe

