IF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
ENDIF()

# it is probably incredibly obvious why this doesn't work
# but i'll have to look into it later.
#INCLUDE_DIRECTORIES(${CXXCAM_INCLUDE_DIR})
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/deps/rs274ngc/include
    ${PROJECT_SOURCE_DIR}/deps/camMachine/include
    ${PROJECT_SOURCE_DIR}/deps/camMachine/deps/cxxcam/include
    ${PROJECT_SOURCE_DIR}/deps/camMachine/deps/geom/include
)

ADD_EXECUTABLE(${PROJECT_NAME} jscam.cpp jsmachine.cpp jsgeom.cpp jsrs274ngc.cpp js.cpp platform.cpp jspath.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} v8 rs274ngc camMachine)
