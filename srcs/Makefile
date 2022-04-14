THE_SOURCE = 	main.cpp \
				manageServer/Server.cpp \
				manageServer/AllServers.cpp \
				response/Response.cpp \
				request/Request.cpp \
				tools/tools.cpp \
				parseConfig/parseConfig.cpp \
				parseConfig/serverBlock.cpp \
				parseConfig/serverLocation.cpp \
				cgi/Cgi.cpp

SRCS = $(THE_SOURCE)
OBJS = $(addprefix ${OBJDIR}/,${SRCS:.cpp=.o})

NAME = webserv
DEPF = -MMD -MP
DEP = ${OBJS:.o=.d}
OBJDIR = helper_files
OBJDIR_STD = helper_files_std
CC = c++
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address 
all: ${NAME}

$(NAME)	: ${OBJS}
	${CC} ${CFLAGS} ${DEPF} ${OBJS} -o $@


${OBJDIR}/%.o:%.cpp
	@mkdir -p ${@D}
	${CC} ${CFLAGS} ${DEPF} -c $< -o $@

clean:
	rm -rf ${OBJDIR}
fclean: clean
	${RM} ${NAME}
re: fclean all

.PHONY: all clean fclean re

-include ${DEP}
