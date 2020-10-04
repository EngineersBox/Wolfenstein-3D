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
add_custom_target(archive_logs ALL)
add_custom_target(clean_archives ALL)
add_custom_target(destroy ALL)
add_custom_target(run ALL)
add_custom_command(
    TARGET clean_out
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green \"Removing previous builds\"
	COMMAND echo \"--------------------------------\"
	COMMAND rm -rvf out/*
	COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
    TARGET clean_logs
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Removing logs in \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"log/*\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND [ `ls -A logs/*.log 2>/dev/null | wc -m` == "0" ] && cmake -E cmake_echo_color --red --bold "No logs to remove" || rm -rvf logs/*.log
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"\"
    COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
    TARGET archive_logs
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Archiving logs in \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"log/*\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND [ `ls -A logs/*.log 2>/dev/null | wc -m` == "0" ] && cmake -E cmake_echo_color --red --bold "No logs to archive" || zip -r logs/archive-$$\(date +"%Y-%m-%d_%H-%M-%S"\)\.zip logs/*.log
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"\"
    COMMAND echo \"--------------------------------\\n\"
	COMMAND make clean_logs
)
add_custom_command(
    TARGET clean_archives
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Removing archives in \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"log/*\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND [ `ls -A logs/archive-*.zip 2>/dev/null | wc -m` == "0" ] && cmake -E cmake_echo_color --red --bold "No archives to remove" || rm -rvf logs/archive-*.zip
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"\"
    COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
    TARGET destroy
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Removing \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"out\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"] \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green \"directory\"
	COMMAND echo \"--------------------------------\"
	COMMAND rm -rvf out
	COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
    TARGET run
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Running new build \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"out/${PROJECT_NAME}\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND ./out/${PROJECT_NAME}
	COMMAND echo \"--------------------------------\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green \"Stopped running\"
	COMMAND echo \"\\n\"
)

# Set test source file
set(CATCH_TESTS_SOURCE "test/testCases.cpp")

# Set test output
set(CATCH_TEST_BUILD_OUT "out/tests/testcases")

add_custom_target(build_tests ALL)
add_custom_target(run_tests ALL)
add_custom_target(run_tests_wo ALL)
add_custom_command(
	TARGET build_tests
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Building tests to \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"out/tests/*\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND mkdir -p out
	COMMAND mkdir -p out/tests
	COMMAND time ${CMAKE_CXX_COMPILER} -std=c++${CMAKE_CXX_STANDARD} ${CATCH_TESTS_SOURCE} -o ${CATCH_TEST_BUILD_OUT} | echo "Built tests in "
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"\"
    COMMAND echo \"--------------------------------\\n\"
)
add_custom_command(
	TARGET run_tests
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Running test cases \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"${CATCH_TEST_BUILD_OUT}\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND ./${CATCH_TEST_BUILD_OUT}
	COMMAND echo \"--------------------------------\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green \"Stopped running\"
	COMMAND echo \"\\n\"
)
add_custom_command(
	TARGET run_tests_wo
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green --no-newline \"Running test cases \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white --no-newline \"[\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold --no-newline \"${CATCH_TEST_BUILD_OUT}\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white \"]\"
	COMMAND echo \"--------------------------------\"
	COMMAND ./${CATCH_TEST_BUILD_OUT} --success
	COMMAND echo \"--------------------------------\"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold --no-newline \"\>\> \"
	COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green \"Stopped running\"
	COMMAND echo \"\\n\"
)

# SET THE EXECUTABLE TO THE SOURCE AND LINK LIBRARIES
add_executable(BSP ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )