# General installer settings
SET (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Texts/Intro.txt)
SET (CPACK_RESOURCE_FILE_LICENSE    ${CMAKE_CURRENT_SOURCE_DIR}/Texts/Lincese.txt)
SET (CPACK_PACKAGE_NAME "OJD")

# The OS specific installer settings are set here
if (APPLE)
    add_subdirectory (MacOS)
elseif (Windows)
    add_subdirectory (Windows)
endif()

# Let CPack create our installer
include (CPack)



