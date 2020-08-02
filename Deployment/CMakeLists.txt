# This creates a CPack based, platform specific installer

# The OS specific installer, components and install destinations are set here
if (APPLE)
    add_subdirectory (MacOS)
else()
    add_subdirectory (Windows)
endif()

# Define platform independent installer settings
SET (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Texts/Intro.txt)
SET (CPACK_RESOURCE_FILE_LICENSE    ${CMAKE_CURRENT_SOURCE_DIR}/Texts/Lincese.txt)
SET (CPACK_PACKAGE_NAME "OJD")
SET (CPACK_SOURCE_IGNORE_FILES TRUE)
SET (CPACK_SOURCE_STRIP_FILES TRUE)
SET (CPACK_COMPONENTS_ALL ${FORMATS_TO_BUILD})

# Let CPack create our installer
include (CPack)
