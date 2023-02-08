TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += LINUX

unix {
    LIBS += "../../LEti_Engine/OpenGL/Linux/lib/libglfw3.a"
    LIBS += "../../LEti_Engine/OpenGL/Linux/lib/libGLEW.a"

    LIBS += -lpthread -lGL -lX11 -ldl

    INCLUDEPATH += "../LEti_Engine/OpenGL/Linux/include/"
}


win32 {
    LIBS += "../LEti_Engine/OpenGL/Windows_x64_mingw/lib/libglfw3.a"
    LIBS += "../LEti_Engine/OpenGL/Windows_x64_mingw/lib/libglew32.a"

    LIBS += -lopengl32 \
	-luser32 \
	-lgdi32 \
	-lshell32

    INCLUDEPATH += "../LEti_Engine/OpenGL/Windows_x64_mingw/include/"
}

INCLUDEPATH += "../LEti_Engine/OpenGL/GLM"
INCLUDEPATH += "../LEti_Engine/include/"

INCLUDEPATH += "../L_Variable/include/"
INCLUDEPATH += "../Utility/include/"


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
    include/Buffer.h \
    include/Camera_2D.h \
    include/Event_Controller.h \
    include/Math_Stuff.h \
    include/Object_System/Debug_Drawable_Frame.h \
    include/Object_System/Modules/Default_Draw_Module_2D.h \
    include/Object_System/Modules/Default_Physics_Module_2D.h \
    include/Object_System/Modules/Draw_Module_Base.h \
    include/Object_System/Modules/Physics_Module_Base.h \
    include/Object_System/Modules/Physics_Module__Rigid_Body_2D.h \
    include/Object_System/Object_2D.h \
    include/Object_System/Object_Base.h \
    include/Object_System/Rigid_Body_2D.h \
    include/Object_System/Text_Field.h \
    include/Physics/Broad_Phase_Interface.h \
    include/Physics/Collision_Detector_2D.h \
    include/Physics/Collision_Resolution__Rigid_Body_2D.h \
    include/Physics/Collision_Resolution__Rigid_Body_2D.h \
    include/Physics/Collision_Resolver.h \
    include/Physics/Default_Narrow_CD.h \
    include/Physics/Default_Narrowest_CD.h \
    include/Physics/Narrow_Phase_Interface.h \
    include/Physics/Narrowest_Phase_Interface.h \
    include/Physics/Physical_Model_2D.h \
    include/Physics/Physical_Model_3D.h \
    include/Physics/SAT_Narrowest_CD.h \
    include/Physics/Space_Hasher_2D.h \
    include/Picture.h \
    include/Picture_Manager.h \
    include/Shader.h \
    include/Texture.h \
    include/Timer.h \
    include/Vertices.h \
    include/Window_Controller.h

SOURCES += \
    source/Buffer.cpp \
    source/Camera_2D.cpp \
    source/Event_Controller.cpp \
    source/Math_Stuff.cpp \
    source/Object_System/Debug_Drawable_Frame.cpp \
    source/Object_System/Modules/Default_Draw_Module_2D.cpp \
    source/Object_System/Modules/Default_Physics_Module_2D.cpp \
    source/Object_System/Modules/Draw_Module_Base.cpp \
    source/Object_System/Modules/Physics_Module_Base.cpp \
    source/Object_System/Modules/Physics_Module__Rigid_Body_2D.cpp \
    source/Object_System/Object_2D.cpp \
    source/Object_System/Object_Base.cpp \
    source/Object_System/Rigid_Body_2D.cpp \
    source/Object_System/Text_Field.cpp \
    source/Physics/Broad_Phase_Interface.cpp \
    source/Physics/Collision_Detector_2D.cpp \
    source/Physics/Collision_Resolution__Rigid_Body_2D.cpp \
    source/Physics/Collision_Resolver.cpp \
    source/Physics/Collision_Resolver.cpp \
    source/Physics/Default_Narrow_CD.cpp \
    source/Physics/Default_Narrowest_CD.cpp \
    source/Physics/Narrow_Phase_Interface.cpp \
    source/Physics/Narrowest_Phase_Interface.cpp \
    source/Physics/Physical_Model_2D.cpp \
    source/Physics/Physical_Model_3D.cpp \
    source/Physics/SAT_Narrowest_CD.cpp \
    source/Physics/Space_Hasher_2D.cpp \
    source/Picture.cpp \
    source/Picture_Manager.cpp \
    source/Shader.cpp \
    source/Texture.cpp \
    source/Timer.cpp \
    source/Vertices.cpp \
    source/Window_Controller.cpp
