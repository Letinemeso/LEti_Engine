TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += LINUX

unix {
    LIBS += "../LEti_Engine/libLEti_Engine.a"

    LIBS += "../../LEti_Engine/OpenGL/Linux/lib/libglfw3.a"
    LIBS += "../../LEti_Engine/OpenGL/Linux/lib/libGLEW.a"

    LIBS += -lpthread -lGL -lX11 -ldl

    INCLUDEPATH += "../LEti_Engine/OpenGL/Linux/include/"
}


win32 {
    LIBS += "../LEti-engine/OpenGL/Windows_x64_mingw/lib/libglfw3.a"
    LIBS += "../LEti-engine/OpenGL/Windows_x64_mingw/lib/libglew32.a"

    LIBS += -lopengl32 \
	-luser32 \
	-lgdi32 \
	-lshell32

    INCLUDEPATH += "OpenGL/Windows_x64_mingw/include/"
}

INCLUDEPATH += "../LEti_Engine/OpenGL/GLM"
INCLUDEPATH += "../LEti_Engine/include/"

DISTFILES += \
    resources/font/bgr.png \
    resources/font/font.png \
    resources/font/font_data.mdl \
    resources/font/font_yellow.png \
    resources/font/text_field.mdl \
    resources/fragment_shader.shader \
    resources/models/quad.mdl \
    resources/models/ymany.mdl \
    resources/textures/plug.png \
    resources/textures/ymany.png \
    resources/vertex_shader.shader

HEADERS += \
    Debug.h \
    include/Buffer.h \
    include/Camera.h \
    include/EIPF.h \
    include/Event_Controller.h \
    include/Math_Stuff.h \
    include/Message_Translator.h \
    include/Object_System/Debug_Drawable_Frame.h \
    include/Object_System/Modules/Default_Draw_Module_2D.h \
    include/Object_System/Modules/Default_Physics_Module_2D.h \
    include/Object_System/Modules/Draw_Module_Base.h \
    include/Object_System/Modules/Physics_Module_Base.h \
    Object_System/Object_2D.h \
    include/Object_System/Object_2D_Base.h \
    include/Object_System/Object_Base.h \
    include/Object_System/Text_Field.h \
    include/Physics/Broad_Phase_Interface.h \
    include/Physics/Default_Narrow_CD.h \
    include/Physics/Default_Narrowest_CD.h \
    include/Physics/Narrow_Phase_Interface.h \
    include/Physics/Narrowest_Phase_Interface.h \
    include/Physics/Physical_Model_2D.h \
    include/Physics/Physical_Model_3D.h \
    include/Physics/Space_Hasher_2D.h \
    include/Physics/Space_Splitter_2D.h \
    include/Picture.h \
    include/Resource_Loader.h \
    include/Shader.h \
    include/Texture.h \
    include/Timer.h \
    include/Tree.h \
    include/Vertices.h \
    include/Window_Controller.h

SOURCES += \
    Debug.cpp \
    source/Buffer.cpp \
    source/Camera.cpp \
    source/EIPF.cpp \
    source/Event_Controller.cpp \
    source/Math_Stuff.cpp \
    source/Object_System/Debug_Drawable_Frame.cpp \
    source/Object_System/Modules/Default_Draw_Module_2D.cpp \
    source/Object_System/Modules/Default_Physics_Module_2D.cpp \
    source/Object_System/Modules/Draw_Module_Base.cpp \
    source/Object_System/Modules/Physics_Module_Base.cpp \
    source/Object_System/Object_2D.cpp \
    source/Object_System/Object_2D_Base.cpp \
    source/Object_System/Object_Base.cpp \
    source/Object_System/Text_Field.cpp \
    source/Physics/Broad_Phase_Interface.cpp \
    source/Physics/Default_Narrow_CD.cpp \
    source/Physics/Default_Narrowest_CD.cpp \
    source/Physics/Narrow_Phase_Interface.cpp \
    source/Physics/Narrowest_Phase_Interface.cpp \
    source/Physics/Physical_Model_2D.cpp \
    source/Physics/Physical_Model_3D.cpp \
    source/Physics/Space_Hasher_2D.cpp \
    source/Physics/Space_Splitter_2D.cpp \
    source/Picture.cpp \
    source/Resource_Loader.cpp \
    source/Shader.cpp \
    source/Texture.cpp \
    source/Timer.cpp \
    source/Tree.cpp \
    source/Vertices.cpp \
    source/Window_Controller.cpp
