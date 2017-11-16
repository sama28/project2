####################################################################
#
#	This is the Makefile for creating MINIREL.
#
#
####################################################################


# where to look for the files

HOME = /home/ameya/project/project2

INCLUDE  = $(HOME)/include

ALGEBRA	 = $(HOME)/algebra
SCHEMA	 = $(HOME)/schema
PHYSICAL = $(HOME)/physical

PARSER   = $(HOME)/frontend



DFLAG =
CFLAGS = -g $(DFLAG) -I$(INCLUDE)
# -g to generate symbolic info. used by
#    the debugger.
# DFLAG can be used to turn specific debug
# messages 'on' or 'off'.  Read up on 
# "#ifdef" for more details


#############################################################
# 	stuff to build the executables
#############################################################

SRCS =  main.cpp

OBJ = main.o 

proj :
	@make ALGEBRA.o
	@make SCHEMA.o
	@make PHYSICAL.o
	@make minirel

ALGEBRA.o :
	@cd $(ALGEBRA) ; make $@
	
SCHEMA.o :
	@cd $(SCHEMA) ; make $@

PHYSICAL.o :
	@cd $(PHYSICAL) ; make $@

FES.o :
	@cd $(PARSER) ; make $@


minirel : $(ALGEBRA)/ALGEBRA.o $(SCHEMA)/SCHEMA.o $(PHYSICAL)/PHYSICAL.o $(PARSER)/FES.o $(OBJ) 
	@echo generating $@
	g++ -std=c++11 $(CFLAGS) $(ALGEBRA)/ALGEBRA.o $(SCHEMA)/SCHEMA.o $(PHYSICAL)/PHYSICAL.o $(PARSER)/FES.o $(OBJ) -o $@


$(OBJ) :  $(INCLUDE)/globals.h $(INCLUDE)/defs.h $(INCLUDE)/error.h $(INCLUDE)/mrdtypes.h $(INCLUDE)/fncn.h

###############################################################
#  misc
###############################################################

clean:  # remove all .o files and other garbage
	rm *.o


