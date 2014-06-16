TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11
#CXXFLAGS += -std=c++0x
CXXFLAGS += -Wall -g


LIBS += -lglfw3
#LIBS += -lglfw #???/
win32:LIBS += -glew32
unix:LIBS += -lGLEW
LIBS += -lglut
LIBS += -lGLU -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi
LIBS += -lrt

LIBS += -L/usr/local/lib -lassimp #/usr/include/assimp
#LIBS += -L/home/kaeon/GL/AntTweakBar/src  # no need to add "l" at first
#LIBS += -L/home/kaeon/GL/AntTweakBar/lib -lAntTweakBar

#LIBS +=


#LIBS += -lAntTweakBar;
#LIBS += -lassimp #/usr/include/assimp
# add -L and the path to fix the problem
# http://stackoverflow.com/questions/718447/adding-external-library-into-qt-creator-project

INCLUDEPATH += R:\opengl\GL
#INCLUDEPATH += /home/kaeon/GL/AntTweakBar/include

SOURCES += main.cpp \
    ../GL/common/texture.cpp \
    ../GL/common/shader.cpp \
    ../GL/common/controls.cpp


HEADERS += \
    ../GL/common/texture.hpp \
    ../GL/common/shader.hpp \
    ../GL/common/controls.hpp



# using anttweak should use this static library !!!!!!!!!!!1

unix:!macx: LIBS += -L$$PWD/../../OpenGL-tutorial_v0014_33/build/external/ -lANTTWEAKBAR_116_OGLCORE_GLFW

INCLUDEPATH += $$PWD/../../OpenGL-tutorial_v0014_33/external/AntTweakBar-1.16/include
DEPENDPATH += $$PWD/../../OpenGL-tutorial_v0014_33/external/AntTweakBar-1.16/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../OpenGL-tutorial_v0014_33/build/external/libANTTWEAKBAR_116_OGLCORE_GLFW.a
