
file(GLOB_RECURSE MyTinyServer_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)


add_library(MyTinyServer_shared SHARED ${MyTinyServer_sources})



