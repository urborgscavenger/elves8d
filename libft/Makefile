# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wilisson <wilisson@student.42heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/15 12:29:45 by wilisson          #+#    #+#              #
#    Updated: 2025/07/25 00:22:15 by wilisson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

SRCS = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
		ft_toupper.c ft_tolower.c ft_strncmp.c ft_strlen.c ft_strlcat.c \
		ft_bzero.c ft_memset.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
		ft_strchr.c ft_strrchr.c ft_memchr.c ft_strnstr.c ft_atoi.c \
		ft_memcmp.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
		ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \

B_SRCS = 	ft_lstnew.c		ft_lstadd_front.c	ft_lstsize.c	ft_lstlast.c \
			ft_lstdelone.c	ft_lstadd_back.c	ft_lstdelone.c	ft_lstclear.c \
			ft_lstiter.c	ft_lstmap.c \
		
OBJS = $(SRCS:.c=.o)

B_OBJS = $(B_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(OBJS) $(B_OBJS)
	ar rcs $(NAME) $^


.PHONY:			all clean fclean re bonus