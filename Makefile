CC = gcc
CFLAGS = -c -Wall -D MAGICKCORE_HDRI_ENABLE=0 -D MAGICKCORE_QUANTUM_DEPTH=16
LDFLAGS =

SOURCES = \
    $(wildcard src/*.c) \
    $(wildcard src/config/*.c) \
    $(wildcard src/utils/*.c) \
    $(wildcard src/http/*.c) \
    $(wildcard src/image/*.c)
    
INSTALL_FOLDERS = \
    bin \
    cache \
    etc \
    log \
    www \
    
INSTALL_PATH=/usr/local/AdaptiveServer/
INSTALL_PATH_BIN_LN=/usr/local/bin/
   
include_libwurfl = -I /usr/local/include
lib_libwurfl = -L /usr/local/lib -l wurfl

include_magickwand = -I /usr/local/include/ImageMagick-7
lib_magickwand = -L /usr/local/lib -l MagickWand-7.Q16HDRI
	
INC = $(include_magickwand) $(include_libwurfl)
LIB = $(lib_magickwand) $(lib_libwurfl) -lm -lpthread

OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = bin/adaptiveserver

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(INC) $(LIB) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

cleanobjs:
	rm -f $(OBJECTS)
	
	
install:
	sudo rm -rf $(INSTALL_PATH)
	sudo mkdir -p $(INSTALL_PATH)
	$(foreach var,$(INSTALL_FOLDERS),sudo cp -r $(var) $(INSTALL_PATH);)
	sudo cp start.sh $(INSTALL_PATH)
	sudo chmod -R 777 $(INSTALL_PATH)
	sudo ln -sf $(INSTALL_PATH)start.sh $(INSTALL_PATH_BIN_LN)/adaptiveserver
