import QtQuick

Item {
    // Reference resolution for UI layout.
    property int referenceHeight: 1280
    property int referenceWidth: 720

    // The root item whose size is used for scaling. This should be set to gameViewport.
    property Item target

    // Scaling factors. UI scale should be based mostly on height for consistency.
    property real scale: target ? target.height / referenceHeight : 1.0

    // Helpers
    function px(value) { return value * scale }
    function font(value) { return value * scale }
}
