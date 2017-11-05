CC = clang++
PROG = toy
SRCS = toy.cpp common.cpp lexer.cpp parser.cpp
LDFLAGS = -L/usr/local/opt/llvm/lib -Wl -rpath /usr/local/opt/llvm/lib
CPPFLAGS = -I/usr/local/opt/llvm/include

