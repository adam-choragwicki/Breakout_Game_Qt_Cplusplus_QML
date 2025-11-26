import QtQuick

Text {
    id: textOverlay
    objectName: "textOverlay" // Required for C++ lookup
    anchors.centerIn: parent

    font.pixelSize: uiScaler.font(48)
    font.bold: true
    color: "white"
    wrapMode: Text.WordWrap
}
