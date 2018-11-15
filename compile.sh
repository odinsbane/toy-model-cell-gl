#!/bin/bash

GLFWDIR=/home/melkor/local/glfw3

export GLFW_PC="$GLFWDIR"/lib/pkgconfig/glfw3.pc
export GLFW_CFLAGS=$(pkg-config --cflags "$GLFW_PC")
export GLFW_LIBS=$(pkg-config --static --libs "$GLFW_PC")


g++ -std=c++11 -Isrc $GLFW_CFLAGS \
 -L"$GLFWDIR"/lib \
  src/*.cpp \
  $GLFW_LIBS -lGLEW -lGL -ltiff -lpthread \
-o build/show_mesh

