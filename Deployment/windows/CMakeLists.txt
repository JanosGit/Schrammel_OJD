# Install VST3 and optionally AAX relative to the Common Files directory
install (TARGETS OJD_VST3 LIBRARY DESTINATION VST3 COMPONENT VST3)
if (AAX IN_LIST FORMATS_TO_BUILD)
    install (TARGETS OJD_AAX LIBRARY DESTINATION Avid/Audio/Plug-Ins COMPONENT AAX)
endif()

SET (CMAKE_INSTALL_PREFIX "C:/Program Files/Common Files/" PARENT_SCOPE)

# We want to use NSIS installer on Windows
SET (CPACK_GENERATOR NSIS PARENT_SCOPE)
