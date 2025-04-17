echo "Compiling source code..."
g++ -o main main.cpp \
            source/dependancy/glad.c \
            source/constants.cpp \
            source/camera.cpp \
            source/light.cpp \
            source/shader.cpp \
            -lGL -lglfw