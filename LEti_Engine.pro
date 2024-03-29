TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

unix {
    INCLUDEPATH += "../LEti_Engine/OpenGL/Linux/include/"
}


win32 {
    INCLUDEPATH += "../LEti_Engine/OpenGL/Windows_x64_mingw/include/"
}

INCLUDEPATH += "OpenGL/GLM"
INCLUDEPATH += "include/"

INCLUDEPATH += "../L_Variable/include/"
INCLUDEPATH += "../L_Utility/include/"


HEADERS += \
    include/FPS_Timer.h \
    include/Math_Stuff.h \
    include/Module.h \
    include/Object_System/Object.h \
    include/Object_System/Object_Base.h \
    include/Transformation_Data.h

SOURCES += \
    source/FPS_Timer.cpp \
    source/Math_Stuff.cpp \
    source/Module.cpp \
    source/Object_System/Object.cpp \
    source/Object_System/Object_Base.cpp \
    source/Transformation_Data.cpp
