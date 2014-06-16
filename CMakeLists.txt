# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (Tutorials)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
        external/AntTweakBar-1.16/include/
	external/glfw-3.0.3/include/GLFW/
	external/glm-0.9.4.0/
	external/glew-1.9.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)
#-static-libgcc
set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)



# Tutorial 2
add_executable(tutorial02_red_triangle 
	tutorial02_red_triangle/tutorial02.cpp
	common/shader.cpp
	common/shader.hpp
	
	tutorial02_red_triangle/SimpleFragmentShader.fragmentshader
	tutorial02_red_triangle/SimpleVertexShader.vertexshader
#-static-libstdc++
)
target_link_libraries(tutorial02_red_triangle
	${ALL_LIBS}
-static-libstdc++ #要用這個東西靜態編譯
)
#add_library(${LIB}_static STATIC ${${LIB}_srcs})

# Xcode and Visual working directories
set_target_properties(tutorial02_red_triangle PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/")
create_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/")
create_default_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/") # tut 1 is not the default or people would complain that tut 2 doesn't work


# My End mill animation
add_executable(EndmillRender 
	EndmillRender/main.cpp
	EndmillRender/millerrender.hpp
	EndmillRender/millerrender.cpp
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
    common/controls.cpp
    common/texture.hpp
	common/controls.hpp
)

#FIND_LIBRARY(TOURTRE_LIBRARY AntTweakBar C:\Users\Kaeon\Desktop\opengl\OpenGL-tutorial_v0014_33\external\AntTweakBar\lib)

#FIND_LIBRARY(TOURTRE_LIBRARY glut32 C:/Users/Kaeon/Desktop/opengl/OpenGL-tutorial_v0014_33/external/glut-3.7.6/)
#TARGET_LINK_LIBRARIES(testOVAS ${TOURTRE_LIBRARY})

target_link_libraries(EndmillRender
	${ALL_LIBS}
	-static-libstdc++ #要加這行來靜態編譯, 不然windows下會找不到dll
	ANTTWEAKBAR_116_OGLCORE_GLFW #連結 antweak
	assimp # 連結assimp
  #  -lglut
        #${TOURTRE_LIBRARY}
)

# Xcode and Visual working directories
set_target_properties(EndmillRender PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/")
create_target_launcher(EndmillRender WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/")
create_default_target_launcher(EndmillRender WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/") # tut 1 is not the default or people would complain that tut 2 doesn't work



# Xcode and Visual working directories
set_target_properties(EndmillRender PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/")
create_target_launcher(EndmillRender WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/")
create_default_target_launcher(EndmillRender WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/") # tut 1 is not the default or people would complain that tut 2 doesn't work





SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET tutorial02_red_triangle POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial02_red_triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/"
)

add_custom_command(
   TARGET EndmillRender POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/EndmillRender${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/EndmillRender/"
)


elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

