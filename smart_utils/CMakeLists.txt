
file(GLOB_RECURSE SMART_UTILS_SRC_FILES ./*.cpp)

add_library(SmartUtils SHARED ${SMART_UTILS_SRC_FILES})

install (TARGETS SmartUtils DESTINATION CMakeBuild)



