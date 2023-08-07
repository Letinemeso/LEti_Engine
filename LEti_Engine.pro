TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

unix {
    LIBS += "LEti_Engine/OpenGL/Linux/lib/libglfw3.a"
    LIBS += "LEti_Engine/OpenGL/Linux/lib/libGLEW.a"

    LIBS += -lpthread -lGL -lX11 -ldl

    INCLUDEPATH += "../LEti_Engine/OpenGL/Linux/include/"
}


win32 {
    LIBS += "OpenGL/Windows_x64_mingw/lib/libglfw3.a"
    LIBS += "OpenGL/Windows_x64_mingw/lib/libglew32.a"

    LIBS += -lopengl32 \
	-luser32 \
	-lgdi32 \
	-lshell32

    INCLUDEPATH += "../LEti_Engine/OpenGL/Windows_x64_mingw/include/"
}

INCLUDEPATH += "OpenGL/GLM"
INCLUDEPATH += "include/"

INCLUDEPATH += "../L_Variable/include/"
INCLUDEPATH += "../L_Utility/include/"
INCLUDEPATH += "../L_Renderer/include/"
INCLUDEPATH += "../L_Physics/include/"


HEADERS += \
    include/Math_Stuff.h \
    include/Object_System/Builder_Stub.h \
    include/Object_System/Debug_Drawable_Frame.h \
    include/Object_System/Modules/Default_Draw_Module_2D.h \
    include/Object_System/Object_2D.h \
    include/Object_System/Object_Base.h \
    include/Object_System/Rigid_Body_2D_Stub.h \
    include/Object_System/Text_Field.h \

SOURCES += \
    source/Math_Stuff.cpp \
    source/Object_System/Builder_Stub.cpp \
    source/Object_System/Debug_Drawable_Frame.cpp \
    source/Object_System/Object_2D.cpp \
    source/Object_System/Object_Base.cpp \
    source/Object_System/Rigid_Body_2D_Stub.cpp \
    source/Object_System/Text_Field.cpp \
