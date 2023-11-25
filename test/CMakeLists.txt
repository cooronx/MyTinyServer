
file(GLOB Server_Test_Sources "${PROJECT_SOURCE_DIR}/test/*.cpp")



foreach(test_source ${Server_Test_Sources})

    # Create a human readable name.
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_source})

    target_link_libraries(${test_name} MyTinyServer_shared)

    # Set test target properties and dependencies.
    set_target_properties(${test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${test_name}
    )

  
endforeach(test_source ${Server_Test_Sources})
