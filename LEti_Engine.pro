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


HEADERS += \
    include/Math_Stuff.h \
    include/Object_System/Builder_Stub.h \
    include/Object_System/Debug_Drawable_Frame.h \
    include/Object_System/Modules/Default_Draw_Module_2D.h \
    include/Object_System/Modules/Dynamic_Physics_Module_2D.h \
    include/Object_System/Modules/Physics_Module_Base.h \
    include/Object_System/Modules/Physics_Module__Rigid_Body_2D.h \
    include/Object_System/Object_2D.h \
    include/Object_System/Object_Base.h \
    include/Object_System/Rigid_Body_2D_Stub.h \
    include/Object_System/Text_Field.h \
    include/Physics/Intersection_Data.h \
    include/Physics/Physical_Models/Physical_Model_2D.h \
    include/Physics/Physical_Models/Physical_Model_3D.h \
    include/Physics/Broad_Phase_Interface.h \
    include/Physics/Collision_Detector_2D.h \
    include/Physics/Collision_Resolution__Rigid_Body_2D.h \
    include/Physics/Collision_Resolution__Rigid_Body_2D.h \
    include/Physics/Collision_Resolver.h \
    include/Physics/Default_Narrowest_CD.h \
    include/Physics/Dynamic_Narrow_CD.h \
    include/Physics/Narrow_Phase_Interface.h \
    include/Physics/Narrowest_Phase_Interface.h \
    include/Physics/Physical_Models/Polygon.h \
    include/Physics/Physical_Models/Rigid_Body_Physical_Model_2D.h \
    include/Physics/Physical_Models/Rigid_Body_Polygon.h \
    include/Physics/SAT_Narrowest_CD.h \
    include/Physics/Space_Hasher_2D.h \

SOURCES += \
    source/Math_Stuff.cpp \
    source/Object_System/Builder_Stub.cpp \
    source/Object_System/Debug_Drawable_Frame.cpp \
    source/Object_System/Modules/Dynamic_Physics_Module_2D.cpp \
    source/Object_System/Modules/Physics_Module_Base.cpp \
    source/Object_System/Modules/Physics_Module__Rigid_Body_2D.cpp \
    source/Object_System/Object_2D.cpp \
    source/Object_System/Object_Base.cpp \
    source/Object_System/Rigid_Body_2D_Stub.cpp \
    source/Object_System/Text_Field.cpp \
    source/Physics/Intersection_Data.cpp \
    source/Physics/Physical_Models/Physical_Model_2D.cpp \
    source/Physics/Physical_Models/Physical_Model_3D.cpp \
    source/Physics/Broad_Phase_Interface.cpp \
    source/Physics/Collision_Detector_2D.cpp \
    source/Physics/Collision_Resolution__Rigid_Body_2D.cpp \
    source/Physics/Collision_Resolver.cpp \
    source/Physics/Collision_Resolver.cpp \
    source/Physics/Dynamic_Narrow_CD.cpp \
    source/Physics/Default_Narrowest_CD.cpp \
    source/Physics/Narrow_Phase_Interface.cpp \
    source/Physics/Narrowest_Phase_Interface.cpp \
    source/Physics/Physical_Models/Polygon.cpp \
    source/Physics/Physical_Models/Rigid_Body_Physical_Model_2D.cpp \
    source/Physics/Physical_Models/Rigid_Body_Polygon.cpp \
    source/Physics/SAT_Narrowest_CD.cpp \
    source/Physics/Space_Hasher_2D.cpp \
