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
    include/Math_Stuff.h \
    include/Module.h \
    include/Object_System/Object_2D.h \
    include/Object_System/Object_Base.h \
    include/Object_System/Text_Field.h \
    include/Transformation_Data.h

SOURCES += \
    source/Math_Stuff.cpp \
    source/Module.cpp \
    source/Object_System/Object_2D.cpp \
    source/Object_System/Object_Base.cpp \
    source/Object_System/Text_Field.cpp \
    source/Transformation_Data.cpp
