cmake_minimum_required(VERSION 3.10)

project(RayTracingInOneWeekend)

# On Linux, default to Release if not specified.
if (UNIX AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Specifies the build type on single-configuration generators" FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/lib)
endforeach()

if (WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_compile_options("/std:c++17")
	add_compile_options("/fp:fast")
endif()

if (UNIX)
	add_compile_options("-std=c++1z")
	add_compile_options("-Wall")
	add_compile_options("-fvisibility=hidden")
	add_compile_options("-ffast-math")
endif()

find_program (ISPC_EXECUTABLE ispc)

if (NOT ISPC_EXECUTABLE)
	message(FATAL_ERROR "Could not find ISPC executable!" )
else()
	message(STATUS ${ISPC_EXECUTABLE})
endif()

if (UNIX)
	find_package(Threads REQUIRED)
endif()

# Executable definition
set(EXE_NAME book1)
set(ISPC_ARCH "x86-64")
set(ISPC_FLAGS "--opt=disable-assertions" "-O2")
set(ISPC_TARGETS "sse2-i32x4,sse4-i32x4,avx1-i32x8,avx2-i32x8,avx512skx-i32x16")

if (UNIX)
	list(APPEND ISPC_FLAGS --pic)
endif()

set(ISPC_SRC_NAME Renderer)
set(ISPC_HEADER_NAME "${ISPC_SRC_NAME}_ispc.h")
set(ISPC_OBJ_NAME "${ISPC_SRC_NAME}_ispc${CMAKE_CXX_OUTPUT_EXTENSION}")
set(ISPC_KNOWN_TARGETS "sse2" "sse4" "avx1-" "avx2" "avx512skx")

set(HEADER_FILES 
	Camera.h
	Hittable.h
	Material.h
	Random.h
	Vec3.h)
	
set(SRC_FILES 
	main.cpp)
	
set(ISPC_HEADER_FILES
	Camera.ispc.h
	HitRecord.ispc.h
	Hittable.ispc.h
	HittableList.ispc.h
	Material.ispc.h
	Random.ispc.h
	Ray.ispc.h
	Vec3.ispc.h)

# Collect list of expected ISPC outputs.
list(APPEND ISPC_BUILD_OUTPUT_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_HEADER_NAME}")
list(APPEND ISPC_BUILD_OUTPUT_OBJS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_OBJ_NAME}")

foreach (ISPC_TARGET ${ISPC_KNOWN_TARGETS})
	string(FIND ${ISPC_TARGETS} ${ISPC_TARGET} FOUND_TARGET)
	if (${FOUND_TARGET} GREATER -1)
		set(OUTPUT_TARGET ${ISPC_TARGET})
		if (${ISPC_TARGET} STREQUAL "avx1-")
			set(OUTPUT_TARGET "avx")
		endif()
		list(APPEND ISPC_BUILD_OUTPUT_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_SRC_NAME}_ispc_${OUTPUT_TARGET}.h")
		list(APPEND ISPC_BUILD_OUTPUT_OBJS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_SRC_NAME}_ispc_${OUTPUT_TARGET}${CMAKE_CXX_OUTPUT_EXTENSION}")
	endif()
endforeach()

# ISPC command.
add_custom_command(OUTPUT ${ISPC_BUILD_OUTPUT_HEADERS} ${ISPC_BUILD_OUTPUT_OBJS}
	COMMAND ${ISPC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${ISPC_SRC_NAME}.ispc ${ISPC_FLAGS} --target=${ISPC_TARGETS} --arch=${ISPC_ARCH} -h ${ISPC_HEADER_NAME} -o ${ISPC_OBJ_NAME}
	VERBATIM
	DEPENDS ${ISPC_HEADER_FILES}
	DEPENDS "${ISPC_SRC_NAME}.ispc")

# Visual Studio prettiness
if (WIN32)
	# Show ispc source in VS solution.
	set_source_files_properties("${ISPC_SRC_NAME}.ispc" PROPERTIES HEADER_FILE_ONLY TRUE)
	
	# Group ispc files inside Visual Studio
	source_group("ISPC" FILES "${ISPC_SRC_NAME}.ispc" ${ISPC_HEADER_FILES})
	source_group("ISPC Header Files" FILES ${ISPC_BUILD_OUTPUT_HEADERS})
	source_group("ISPC Object Files" FILES ${ISPC_BUILD_OUTPUT_OBJS})
endif()

# Executable
add_executable(${EXE_NAME} ${HEADER_FILES} ${SRC_FILES} ${ISPC_HEADER_FILES} ${ISPC_BUILD_OUTPUT_HEADERS} ${ISPC_BUILD_OUTPUT_OBJS} "${ISPC_SRC_NAME}.ispc")
set_target_properties(${EXE_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if (UNIX)
	target_link_libraries(${EXE_NAME} Threads::Threads)
endif()
