#Compiler and Linker
CC			:=	c++

#The Target Binary Program
TARGET		:=	irc_server
DEBUGOUT	:=	debug.out
KEMAOUT		:=	kema.out
DODOUT		:=	dod.out

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR		:=	Classes
INCDIR		:=	Classes
OBJDIR		:=	obj
TARGETDIR	:=	bin
SRCEXT		:=	cpp
HDEXT		:=	hpp
OBJEXT		:=	o
CHECKDIR	:=	check
CHECKOUTDIR	:=	tmp
# SPECIALLIB	:=	/Users/jjourdan/.brew/Cellar/openssl@3/3.0.1/include/

#Flags, Libraries and Includes
CFLAGS		:=	-Wall -Wextra -Werror -std=c++98
DEBUGFLAG	:=	-Wall -Wextra -fsanitize=address
ARG			:= 	4242 password

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
HEADERS		:=	$(shell find $(INCDIR) -type f -name *.$(HDEXT))
SOURCES		:=	$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
KEMASOURCES	:=	$(shell find $(SRCDIR) -type f -name *.$(SRCEXT) | grep -v Server | grep -v template)
DODSOURCES	:=	$(shell find $(SRCDIR) -type f -name *.$(SRCEXT) | grep -v Server | grep -v template)
OBJECTS		:=	$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
KEMAOBJECTS	:=	$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(KEMASOURCES:.$(SRCEXT)=.$(OBJEXT)))
DODBJECTS	:=	$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(DODSOURCES:.$(SRCEXT)=.$(OBJEXT)))

LOGPATH		:=	$(shell pwd)/log
LOGFILE		:=	$(LOGPATH)/server.log
COMMAND		=	cat Classes/Log/Log.template.hpp | sed 's!@PATHTOLOGFILE!$(LOGFILE)!g' > Classes/Log/Log.hpp

#Default Make
all: directories $(TARGETDIR)/$(TARGET)

#Remake
re:	fclean all

#Make the Directories
directories: template
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(OBJDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(OBJDIR)
	@$(RM) -rf $(CHECKOUTDIR)

#Full Clean, Objects and Binaries
fclean:	clean
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -rf $(KEMAOUT)
	@$(RM) -rf $(LOGPATH)

clean_main:
	@$(RM) -rf $(OBJDIR)
	@$(RM) -rf $(TARGETDIR)

remain: clean_main all

debug: directories $(OBJECTS)
	$(CC) $(DEBUGFLAG) -o $(TARGETDIR)/$(DEBUGOUT) $(OBJECTS)
	@clear
	./$(TARGETDIR)/$(DEBUGOUT) $(ARG)

ddebug:	directories $(OBJECTS)
	$(CC) $(DEBUGFLAG) -o $(TARGETDIR)/$(DEBUGOUT) $(OBJECTS)
	@printf "\ec"
	./$(TARGETDIR)/$(DEBUGOUT) $(ARG)

test: directories $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $(OBJECTS)
	@clear
	./$(TARGETDIR)/$(TARGET) $(ARG)

ttest: directories $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $(OBJECTS)
	@printf "\ec"
	./$(TARGETDIR)/$(TARGET) $(ARG)

leaks: directories $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $(OBJECTS)
	@clear
	leaks --atExit -- ./$(TARGETDIR)/$(TARGET) $(ARG)

check: all
	@mkdir -p $(CHECKOUTDIR)

nc:
	@printf "Launch test client\n"
	@nc 172.0.0.1 6667

template:
	@mkdir -p $(LOGPATH)
	@touch $(LOGFILE)
	@chmod 666 $(LOGFILE)
	@$(COMMAND)

kema: directories $(KEMAOBJECTS)
	@printf "\ec"
	@$(CC) $(KEMAOBJECTS) Classes/Server/kema.cpp -o $(TARGETDIR)/$(KEMAOUT)
	@./$(TARGETDIR)/$(KEMAOUT)

dod: directories $(DODBJECTS)
	@printf "\ec"
	$(CC) $(DODBJECTS) Classes/Server/Server.cpp -o $(TARGETDIR)/$(DODOUT)
	./$(TARGETDIR)/$(DODOUT) $(ARG)

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

#Non-File Targets
.PHONY: all re directories clean fclean clean_main remain debug ddebug test ttest leaks check kema dod nc
