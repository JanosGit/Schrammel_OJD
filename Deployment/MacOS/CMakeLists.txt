
# The targtes to install on MacOS
install (TARGETS OJD_VST3 LIBRARY DESTINATION /Library/Audio/Plug-Ins/VST3       COMPONENT VST3)
install (TARGETS OJD_AU   LIBRARY DESTINATION /Library/Audio/Plug-Ins/Components COMPONENT AU)
if (AAX_SDK_PATH)
    install (TARGETS OJD_AAX LIBRARY DESTINATION "/Library/Application Support/Avid/Audio/Plug-Ins" COMPONENT AAX)
endif()

# Selects all formats that are contained in FORMATS_TO_BUILD as components for the installer
SET (CPACK_COMPONENTS_ALL ${FORMATS_TO_BUILD} PARENT_SCOPE)

# We want to use the native Apple GUI installer
SET (CPACK_GENERATOR productbuild PARENT_SCOPE)



