####################################################################
#	This is the Makefile for PHYSICAL LAYER.
#
#	If you want to add a file, add it to both the SRC and OBJ lists.
#	Use a backslash for continuation into the next line.
#
####################################################################

####################################################################
# FLAGS for the C compiler
#####################################################################

# where to look for the header files
INCLUDE = ../include

DFLAG =
CFLAGS = -g $(DFLAG) -I$(INCLUDE)
# -g to generate symbolic info. used by
#    the debugger.
# DFLAG can be used to turn specific debug
# messages 'on' or 'off'.  Read up on 
# "#ifdef" for more details

SRCS = \
	getslots.cpp error.cpp createcats.cpp filesize.cpp opencats.cpp closecats.cpp openrel.cpp closerel.cpp findrelnum.cpp \
	readpage.cpp flushpage.cpp writerec.cpp findrec.cpp getnextrec.cpp insertrec.cpp deleterec.cpp \
	buffmanage.cpp bread_string.cpp bread_int.cpp

OBJ = \
	getslots.o error.o createcats.o filesize.o opencats.o closecats.o openrel.o closerel.o findrelnum.o \
	readpage.o flushpage.o writerec.o findrec.o getnextrec.o insertrec.o deleterec.o buffmanage.o bread_string.o bread_int.o

#############################################################
# 	stuff to build the executables
#############################################################

PHYSICAL.o : $(OBJ)
	@echo generating $@
	@ld -r $(OBJ) -o $@

$(OBJ) : $(INCLUDE)/globals.h $(INCLUDE)/defs.h $(INCLUDE)/error.h $(INCLUDE)/fncn.h

###############################################################
#  misc
###############################################################

tags:
	ctags -t $(SRCS) $(INCLUDE)/defs.h $(INCLUDE)/error.h

lint:	
	lint $(SRCS) $(INCLUDE)/defs.h $(INCLUDE)/error.h > lint.out

clean:  # remove all .o files and other garbage
	rm *.o

