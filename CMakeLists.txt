cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME BSP)
project(${PROJECT_NAME})

# SET CXX VERSION
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# INCLUDE LIBRARIES
add_compile_options(-stdlib=libc++)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

# SET THE SOURCE FILE
set(SOURCES "src/raytracer/Raytracer.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY out)
FILE(GLOB logs_contents "logs/*.log")

add_custom_target(clean_out ALL)
add_custom_target(clean_logs ALL)
add_custom_target(destroy ALL)
add_custom_target(run ALL)
add_custom_command(
    TARGET clean_out
    COMMAND echo \">> Removing previous builds\"
	COMMAND echo \"--------------------------------\"
	COMMAND rm -rvf out/*
	COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
    TARGET clean_logs
    COMMAND echo \">> Removing logs\"
	COMMAND echo \"--------------------------------\"
    COMMAND rm -rvf logs/*
    COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
    TARGET destroy
    COMMAND echo \">> Removing out directory\"
	COMMAND echo \"--------------------------------\"
	COMMAND rm -rvf out
	COMMAND echo \"--------------------------------\\n\"
)

add_custom_command(
    TARGET run
	COMMAND echo \">> Running new build...\"
	COMMAND echo \"--------------------------------\\n\"
	COMMAND ./out/${PROJECT_NAME}
	COMMAND echo ">> Stopped running"
	COMMAND echo \"--------------------------------\\n\"
)

# SET THE EXECUTABLE TO THE SOURCE AND LINK LIBRARIES
add_executable(BSP ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )