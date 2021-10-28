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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_compile_options("/fp:fast")
endif()

if (UNIX)
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
set(ISPC_FLAGS "--opt=disable-assertions" "-O2")

# Find out the CPU type.
if (UNIX)
    if (NOT ISPC_ARCH)
        execute_process(COMMAND bash "-c" "uname -m | sed -e s/x86_64/x86-64/ -e s/amd64/x86-64/ -e s/arm64/aarch64/" OUTPUT_VARIABLE ispc_arch)
        string(STRIP ${ispc_arch} ispc_arch)
    endif()
else()
    if (NOT ISPC_ARCH)
		set(ispc_arch "x86-64")
	endif()	
endif()

set(ISPC_ARCH "${ispc_arch}" CACHE STRING "ISPC CPU ARCH")


if ("${ISPC_ARCH}" STREQUAL "x86-64")
	# Intel x86-64 CPUs
	set(ISPC_TARGETS "sse2-i32x4,sse4-i32x4,avx1-i32x8,avx2-i32x8,avx512skx-i32x16")
	set(ISPC_TARGETS_OBJS "sse2" "sse4" "avx1" "avx2" "avx512skx")
elseif ("${ISPC_ARCH}" STREQUAL "aarch64")
	# ARM aarch64 CPUs
	set(ISPC_TARGETS "neon-i32x4")
	set(ISPC_TARGETS_OBJS "neon")
else ()
	message(FATAL_ERROR "Unknown ISPC_ARCH '${ISPC_ARCH}'")
endif()

if (UNIX)
	list(APPEND ISPC_FLAGS --pic)
endif()

set(ISPC_SRC_NAME Renderer)
set(ISPC_HEADER_NAME "${ISPC_SRC_NAME}_ispc.h")
set(ISPC_OBJ_NAME "${ISPC_SRC_NAME}_ispc${CMAKE_CXX_OUTPUT_EXTENSION}")

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

# Collect list of expected ISPC outputs (obj file names unfortunately do not exactly match the list of targets, hence the extra ceremonial code).
list(APPEND ISPC_BUILD_OUTPUT_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_HEADER_NAME}")
list(APPEND ISPC_BUILD_OUTPUT_OBJS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_OBJ_NAME}")
list(LENGTH ISPC_TARGETS NUMBER_OF_TARGETS)

if (${ISPC_TARGETS} GREATER 1)
	foreach (ISPC_TARGET ${ISPC_TARGETS_OBJS})
		string(FIND ${ISPC_TARGETS} ${ISPC_TARGET} FOUND_TARGET)
		if (${FOUND_TARGET} GREATER -1)
			set(OUTPUT_TARGET ${ISPC_TARGET})
			if (${ISPC_TARGET} STREQUAL "avx1")
				set(OUTPUT_TARGET "avx")
			endif()
			list(APPEND ISPC_BUILD_OUTPUT_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_SRC_NAME}_ispc_${OUTPUT_TARGET}.h")
			list(APPEND ISPC_BUILD_OUTPUT_OBJS "${CMAKE_CURRENT_BINARY_DIR}/${ISPC_SRC_NAME}_ispc_${OUTPUT_TARGET}${CMAKE_CXX_OUTPUT_EXTENSION}")
	endforeach()
endif()

# ISPC command.
add_custom_command(OUTPUT ${ISPC_BUILD_OUTPUT_HEADERS} ${ISPC_BUILD_OUTPUT_OBJS}
	COMMAND ${ISPC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${ISPC_SRC_NAME}.ispc ${ISPC_FLAGS} --target=${ISPC_TARGETS} --arch=${ISPC_ARCH} -h ${ISPC_HEADER_NAME} -o 
${ISPC_OBJ_NAME}
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
