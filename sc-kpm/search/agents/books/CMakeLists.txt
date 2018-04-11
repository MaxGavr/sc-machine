file(GLOB_RECURSE SOURCES_CPP "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

add_library (search-books SHARED ${SOURCES_CPP} ${HEADERS})

include_directories(${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})
add_dependencies(search-books sc-memory)
target_link_libraries(search-books sc-kpm-common scp)

install_targets("/lib/sc-memory/extensions" search-books)
