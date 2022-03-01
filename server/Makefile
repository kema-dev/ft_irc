#Compiler and Linker
CC			:=	c++

#The Target Binary Program
TARGET		:=	irc_server
DEBUGOUT	:=	debug.out

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR		:=	Sources
INCDIR		:=	Classes
OBJDIR		:=	obj
TARGETDIR	:=	bin
SRCEXT		:=	cpp
HDEXT		:=	hpp
OBJEXT		:=	o
CHECKDIR	:=	check
CHECKOUTDIR	:=	tmp

#Flags, Libraries and Includes
CFLAGS		:=	-Wall -Wextra -Werror -std=c++98
DEBUGFLAG	:=	-Wall -Wextra -fsanitize=address
ARG			:= 	6697 password

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
HEADERS		:=	$(shell find $(INCDIR) -type f -name *.$(HDEXT))
SOURCES		:=	$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS		:=	$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET)

#Remake
re:	fclean all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(OBJDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(OBJDIR)
	@$(RM) -rf $(CHECKOUTDIR)

#Full Clean, Objects and Binaries
fclean:	clean
	@$(RM) -rf $(TARGETDIR)

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
# test 1

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

#Non-File Targets
.PHONY: all re directories clean fclean clean_main remain debug ddebug test ttest leaks check