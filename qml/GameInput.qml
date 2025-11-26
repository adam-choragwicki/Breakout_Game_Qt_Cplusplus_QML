import QtQuick

Item {
    id: gameInput
    anchors.fill: parent

    // For mouse movement → paddle control
    MouseArea {
        id: mouseArea
        objectName: "mainWindowMouseArea"
        anchors.fill: parent
        hoverEnabled: true
        enabled: false

        onPositionChanged: (mouse) => {
            Controller.processMouseMove(mouse.x, parent.width)
        }

        onClicked: (mouse) => {
            Controller.processMouseClick(mouse.button)
        }
    }

    // For keyboard events
    FocusScope {
        id: gameFocusScope
        focus: true

        Keys.onPressed: (event) => {
            Controller.processKeyPress(event.key)
            event.accepted = true
        }

        function refocus() {
            gameFocusScope.forceActiveFocus()
        }
    }

    // Expose helper methods to parent
    function refocus() {
        gameFocusScope.forceActiveFocus()
    }
}
