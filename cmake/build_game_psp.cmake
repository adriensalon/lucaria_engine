function(add_lucaria_game_psp TARGET)
    set(options)
    set(one_value_args INSTALL_DIR)
    set(multi_value_args SOURCES INCLUDES DEFINES BUILD_ARGS)
    cmake_parse_arguments(PARSE_ARGV 0 LBG "${options}" "${one_value_args}" "${multi_value_args}")

    find_program(PSP_CMAKE_EXECUTABLE psp-cmake)

    if(NOT PSP_CMAKE_EXECUTABLE)
        message(STATUS "PlayStationPortable SDK not found. ${TARGET}_psp will not be built")
        return()
    endif()
	
	get_filename_component(_pspsdk_dir "${PSP_CMAKE_EXECUTABLE}" DIRECTORY)
	get_filename_component(_pspsdk_root "${_pspsdk_dir}/.." ABSOLUTE)
	message(STATUS "Found PlayStationPortable SDK at ${_pspsdk_root}")

    set(_old_lucaria_game_cmake_command "${LUCARIA_GAME_CMAKE_COMMAND}")
    set(LUCARIA_GAME_CMAKE_COMMAND "${PSP_CMAKE_EXECUTABLE}")

    lucaria_build_game(
        "psp"
        "${CMAKE_GENERATOR}"
        "" # no toolchain, psp-cmake handles it
        "${TARGET}"
        "${LBG_SOURCES}"
        "${LBG_INSTALL_DIR}"
        "${LBG_INCLUDES}"
        "${LBG_DEFINES}"
        "${LBG_BUILD_ARGS}"
        "-DLUCARIA_PACKAGE=OFF;-DBUILD_PRX=1;-DENC_PRX=1"
    )

    set(LUCARIA_GAME_CMAKE_COMMAND "${_old_lucaria_game_cmake_command}")

    if(LBG_INSTALL_DIR)
        add_custom_command(
            TARGET ${TARGET}_psp
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LBG_INSTALL_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}/psp/EBOOT.PBP"
                "${LBG_INSTALL_DIR}/EBOOT.PBP"
        )
    endif()

endfunction()