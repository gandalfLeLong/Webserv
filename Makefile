NAME	= Webserv

#----------DIRECTORIES-----------#

SRC_DIR	= sources/
OBJ_DIR	= obj/
INC_DIR	= includes/

#----------HEADERS-----------#

HEADER	= ${INC_DIR}Webserv.hpp \
		  ${INC_DIR}server/Client.hpp \
          ${INC_DIR}server/Server.hpp \
		  ${INC_DIR}server/Data.hpp \
          ${INC_DIR}io_manip/Header.hpp \
          ${INC_DIR}io_manip/Request.hpp \
          ${INC_DIR}io_manip/Response.hpp \
          ${INC_DIR}config/ConfigParser.hpp \

INCLUDE = -I ${INC_DIR} \
		  -I ${INC_DIR}server \
		  -I ${INC_DIR}io_manip \
		  -I ${INC_DIR}config \

#----------SOURCE FILES-----------#

SRCS		= main.cpp
SRC			= ${addprefix ${SRC_DIR}, ${SRCS}}

SERV_SRCS	= Client.cpp \
			  Server.cpp \
			  Data.cpp
SERV_SRC	= ${addprefix ${SRC_DIR}server/, ${SERV_SRCS}}

IO_SRCS		= Header.cpp \
			  Request.cpp \
			  Response.cpp
IO_SRC		= ${addprefix ${SRC_DIR}io_manip/, ${IO_SRCS}}

CONFIG_SRCS	= ConfigParser.cpp
CONFIG_SRC	= ${addprefix ${SRC_DIR}config/, ${CONFIG_SRCS}}

UTILS_SRCS	= utils.cpp
UTILS_SRC	= ${addprefix ${SRC_DIR}utils/, ${UTILS_SRCS}}

ALL_SRC		= ${SRC} ${SERV_SRC} ${CONFIG_SRC} ${UTILS_SRC} ${IO_SRC}

OBJ	= ${ALL_SRC:${SRC_DIR}%.cpp=${OBJ_DIR}%.o}

#----------COMPILATION----------#

CC	= c++

CFLAGS	= -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g 


RM	= rm -rf

#----------RULES----------#

${OBJ_DIR}%.o: ${SRC_DIR}%.cpp
	@mkdir -p $(@D)  # Ensure subdirectories are created
	${CC} ${CFLAGS} ${INCLUDE} -o $@ -c $<

${NAME}: ${OBJ} ${HEADER}
	${CC} ${CFLAGS} ${OBJ} ${INCLUDE} -o ${NAME}

all: ${NAME}

clean:
	@${RM} ${OBJ_DIR}

fclean: clean
	@${RM} ${NAME} .vscode configFile.replace

re: fclean all

.PHONY: all clean fclean re
