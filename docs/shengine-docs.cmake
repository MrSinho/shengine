cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



# 
# DOCS GENERATION
# 
function(build_shengine_docs)



if (NOT DEFINED SH_ENGINE_BINARIES_DIR)
message(FATAL_ERROR "shengine cmake error: missing SH_GUI_BINARIES_DIR")
endif()

if (NOT DEFINED SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "shengine cmake error: missing SH_ENGINE_ROOT_DIR")
endif()




find_package(Doxygen)


if (DOXYGEN_FOUND)

    set(SH_ENGINE_DOXYFILESRC ${SH_ENGINE_ROOT_DIR}/docs/Doxyfile.in)
    set(SH_ENGINE_DOXYFILEDST ${SH_ENGINE_ROOT_DIR}/docs/Doxyfile)

    message(STATUS "Configuring " ${SH_ENGINE_DOXYFILEDST} " from " ${SH_ENGINE_DOXYFILESRC})

    configure_file(
        ${SH_ENGINE_DOXYFILESRC} 
        ${SH_ENGINE_DOXYFILEDST}
    )

    message(STATUS "Creating custom docs command: " ${DOXYGEN_EXECUTABLE} " " ${SH_ENGINE_DOXYFILEDST})

    add_custom_target(docs-shengine
        COMMAND ${DOXYGEN_EXECUTABLE} ${SH_ENGINE_DOXYFILEDST}
        WORKING_DIRECTORY ${SH_ENGINE_ROOT_DIR}
        COMMENT "Generating HTML documentation"
        VERBATIM
    )

endif(DOXYGEN_FOUND)

endfunction()