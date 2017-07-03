########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : EDIT HERE
# Student Number   : EDIT HERE
# Course Code      : EDIT HERE
# Program Code     : EDIT HERE
# Start up code provided by Paul Miller
########################################################################

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h ppd_shared.h
README=ppd_readme
MAKEFILE=Makefile
CC=gcc
CFLAGS=-ansi -Wall -pedantic
OBJECTS=ppd_main.o ppd_utility.o ppd_stock.o ppd_options.o ppd_menu.o ppd_coin.o
EXE= ppd.exe
DATA=stock.dat coins.dat
LOGFILE=requirement13.txt

#%.o : %.c
#	$(cc) $(CFLAGS) -c $<

ppd: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXE)

run:
	./$(EXE) $(DATA)

val:
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE) $(DATA)

val1:
	valgrind --leak-check=full --show-reachable=yes ./$(EXE) $(DATA)

val2:
	valgrind --track-origins=yes ./$(EXE) $(DATA)

clean:
	rm $(OBJECTS) $(EXE)

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE) $(LOGFILE)
