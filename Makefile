####################################################################################
# Makefile for automatically compiling programs for both the gumstix		########
# and the host pc at the same time. This Makefile makes it simple to 		########
# compile, install, clean up, and execute programs on both the gumstix		########
# and the host pc. To use this Makefile simply copy it to the folder 		########
# that contains all of your ".c" source files. Note: all of your			########
# ".c" files must be in one folder for this Makefile to find them.			########
#																			########
#		Author: Christopher Crews											########
#		Date: Aug. 5, 2011													########
#																			########
#	Commands List:															########
#		make																########
#		make gum															########
#		make pc																########
#		make client															########
#		make server															########
#		make install														########
#		make installall														########
#		make clean															########
#		make cleanex														########
#		make cleanobjs														########
#		make run															########
#		make rungum															########
#		make runpc															########
####################################################################################

####################################################################################
#####	User Changeable Section												########

# library links for gumstix. 
GUM_LIBS = -lpthread -lSDL -lm
# library links for pc
PC_LIBS = -lSDL_gfx -lSDL_ttf -lSDL -lm
# compiler flags
CFLAGS = -Wall

#####	END User Changeable Section ################################################

####################################################################################
#####	All code past here is generic and should not be changed 			########
####################################################################################

# Declare the variable that is used to set the target name at runtime. To use, append
# "T=NAME" to the end of any 'make' command (where NAME is the desired executable
# name, which will have "_gum" and/or "_pc" appended to the end of NAME to denote
# whether it is for the gumstix or the host pc. An example of this command is as
# follows: $ make T=executable                                                  
T =		# leave blank
# Test to see if T is not equal to NULL
ifneq ($(strip $(T)),)
# TARGET = T  if T != NULL
TARGET = ${T}
else
# Default: TARGET = Exec if T == NULL
TARGET = Exec
endif

IP = 192.168.0.4

# Setup seperate targets for the gumstix and host pc
TARGET_GUM = ${TARGET}_gum
TARGET_PC = ${TARGET}_pc

# Setup paths to needed tools and directory's
OVEROTOP = /home/ubuntu/gnome-r13
OETMP = ${OVEROTOP}/tmp
TOOLDIR = ${OETMP}/sysroots/i686-linux/usr/armv7a/bin
STAGEDIR = ${OETMP}/sysroots/armv7a-angstrom-linux-gnueabi/usr
GUM_LIBDIR = ${STAGEDIR}/lib
GUM_INCDIR = ${STAGEDIR}/include
GUM_CC = ${TOOLDIR}/arm-angstrom-linux-gnueabi-${CC}
CC = gcc

# Gumstix object files directory
GUMOBJSDIR := GumObjects
# Host pc object files directory
PCOBJSDIR := PCObjects
# Find all ".c" files in current directory
SRCS := $(wildcard *.c)
# Take the .c source files and create an object file for each source file and add
# the appropriate objects directory prefix
GUMOBJS := $(addprefix $(GUMOBJSDIR)/,$(SRCS:.c=.o))
PCOBJS := $(addprefix $(PCOBJSDIR)/,$(SRCS:.c=.o))

# Build object files for gumstix with NAME.o where NAME corresponds with a NAME.c
$(GUMOBJSDIR)/%.o : %.c
	${GUM_CC} -D _GUM -U _PC -U _CLIENT ${CFLAGS} -I ${GUM_INCDIR} -c $< -o $@

# Build object files for host pc with NAME.o where NAME corresponds with a NAME.c
$(PCOBJSDIR)/%.o : %.c
	${CC} -D _PC -U _GUM -D _CLIENT ${CFLAGS} -c $< -o $@

# "all:" is the target that make runs by default when you type "make" in the command
# line. This will automagically find the .c files, compile the object files and 
# dependencies, link the object files, and compile an executable file for both
# the gumstix and the host PC all at once. Note: some of the targets have been set
# as .PHONY targets to prevent errors from having multiple targets.
all: $(GUMOBJS) $(PCOBJS) ${TARGET_GUM} ${TARGET_PC}
.PHONY: rungum runpc run clean cleanobjs cleanex pc gum client server install installall ${TARGET_GUM} ${TARGET_PC}

# Check to see if "GumObjects" directory exists. If it does exist, expand the object
# files for the gumstix into the directory.
$(GUMOBJS): | $(GUMOBJSDIR)

# If the "GumObjects directory does not exist from above, 'make' will be directed 
# here to create the "GumObjects directory.
$(GUMOBJSDIR):
	mkdir $(GUMOBJSDIR)

# Same as above, but for "PCObjects" directory
$(PCOBJS): | $(PCOBJSDIR)

# Same as above, but for "PCObjects directory
$(PCOBJSDIR):
	mkdir $(PCOBJSDIR)

# Build executable for the gumstix from the gumstix object files
${TARGET_GUM}: ${GUMOBJS}
	${GUM_CC} -D _GUM -U _PC ${GUMOBJS} -L ${GUM_LIBDIR} ${GUM_LIBS} -o $@

# Build executable for host pc from host pc object files
${TARGET_PC}: ${PCOBJS}
	${CC} -D _PC -U _GUM ${PCOBJS} ${PC_LIBS}  -o $@
	
# Builds the host pc client executable
client: ${PCOBJS}
	${CC} -D _CLIENT -U _GUM -U _PC PCObjects/client.o -o $@

# Builds the gumstix server executable
server: ${GUMOBJS}
	${GUM_CC} -D _GUM -U _PC GumObjects/server.o -L ${GUM_LIBDIR} ${GUM_LIBS} -o $@

# Removes all object files and executables. To use, type "make clean" in the command line
clean:	
	rm -f ${TARGET_GUM} ${TARGET_PC} *~
	rm -f ${GUMOBJSDIR}/*.o ${GUMOBJSDIR}/*~
	rm -f ${PCOBJSDIR}/*.o ${PCOBJSDIR}/*~

# Removes ONLY the object files. To use, type "make cleanobjs" in the command line
cleanobjs:
	rm -f ${GUMOBJSDIR}/*.o ${GUMOBJSDIR}/*~
	rm -f ${PCOBJSDIR}/*.o ${PCOBJSDIR}/*~

# Removes only the executables. To use, type "make cleanex" in the command line
cleanex:
	rm -f ${TARGET_GUM} ${TARGET_PC} *~

# Copies the executable to the gumstix. To use, type "make install" in the command line
install:
	scp ${TARGET_GUM} root@${IP}:~/Programs/Exec/exe_program1

# Copies the executable and bitmaps to the Gumstix. To use, type "make install" in the
# command line.
installall:
	scp ${TARGET_GUM} root@${IP}:~/Programs/${TARGET_GUM}
	scp Graphics/*.bmp root@${IP}:~/Programs/Graphics/.

# Executes the executable on the gumstix that was created by typing "make" or "make gum"
# in the command line. To use, type "make rungum" in the command line. Note: Must use 
# "make install" before using "make rungum"
rungum:
	ssh root@${IP} ./Programs/${TARGET} &

# Executes the executable that was created by typing "make" or "make pc" in the command
# line. To use, type "make runpc" in the command line.
runpc:
	./${TARGET_PC} &

# Executes the executables that were created by typing "make" on both the gumstix and
# the host pc at the same time. To use, type "make run" in the command line.
run: rungum runpc

# Compiles, links, and builds an executable for the host pc only. To use, type "make pc"
# in the command line.
pc: ${TARGET_PC}

# Compiles, links, and builds an executable for the gumstix only. To use, type "make gum"
# in the command line.
gum: ${TARGET_GUM}

#####	END ########################################################################



