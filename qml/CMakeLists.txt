set(MODULE_NAME BreakoutQml)
set(LIB_NAME ${MODULE_NAME}Lib) # BreakoutQmlLib

qt_add_library(${LIB_NAME} STATIC)

target_link_libraries(${LIB_NAME} PRIVATE Qt6::Quick)

qt_add_qml_module(${LIB_NAME}
        URI ${MODULE_NAME}
        VERSION 1.0
        OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/BreakoutQml
        QML_FILES
        Main.qml
        GameArea.qml
        GameInput.qml
        TextOverlay.qml
        EndGameOverlay.qml
        EscapeMenuOverlay.qml

        UiScaler.qml
        ScalingUtilities.js
)
