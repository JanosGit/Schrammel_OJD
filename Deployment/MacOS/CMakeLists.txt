
# Install VST3, AU and optionally AAX relative to the system library location
install (TARGETS OJD_VST3 LIBRARY DESTINATION Audio/Plug-Ins/VST3       COMPONENT VST3)
install (TARGETS OJD_AU   LIBRARY DESTINATION Audio/Plug-Ins/Components COMPONENT AU)
if (AAX IN_LIST FORMATS_TO_BUILD)
    install (TARGETS OJD_AAX LIBRARY DESTINATION "Application Support/Avid/Audio/Plug-Ins" COMPONENT AAX)
endif()

SET (CMAKE_INSTALL_PREFIX /Library/ PARENT_SCOPE)

# We want to use the native Apple GUI installer
SET (CPACK_GENERATOR productbuild PARENT_SCOPE)
