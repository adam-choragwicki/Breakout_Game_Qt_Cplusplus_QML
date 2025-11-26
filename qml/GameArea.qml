import QtQuick
import "ScalingUtilities.js" as ScalingUtilities

Rectangle {
    id: gameArea
    color: "black"

    /* Paddle */
    Rectangle {
        id: paddle
        x: ScalingUtilities.scaleX(Controller.paddleX, Controller.gameArenaSize.width, gameArea.width)
        y: ScalingUtilities.scaleY(Controller.paddleY, Controller.gameArenaSize.height, gameArea.height)
        width: ScalingUtilities.scaleX(Controller.paddleWidth, Controller.gameArenaSize.width, gameArea.width)
        height: ScalingUtilities.scaleY(Controller.paddleHeight, Controller.gameArenaSize.height, gameArea.height)
        radius: width * 0.05
        color: "blue"
    }

    /* Ball */
    Rectangle {
        id: ball
        x: ScalingUtilities.scaleX(Controller.ballX, Controller.gameArenaSize.width, gameArea.width)
        y: ScalingUtilities.scaleY(Controller.ballY, Controller.gameArenaSize.height, gameArea.height)
        width: ScalingUtilities.scaleX(Controller.ballDiameter, Controller.gameArenaSize.width, gameArea.width)
        height: ScalingUtilities.scaleY(Controller.ballDiameter, Controller.gameArenaSize.height, gameArea.height)
        radius: width / 2
        color: "yellow"
    }

    /* Bricks */
    Repeater {
        model: Controller.bricksModel
        delegate: Rectangle {
            x: ScalingUtilities.scaleX(model.x, Controller.gameArenaSize.width, gameArea.width)
            y: ScalingUtilities.scaleY(model.y, Controller.gameArenaSize.height, gameArea.height)
            width: ScalingUtilities.scaleX(model.width, Controller.gameArenaSize.width, gameArea.width)
            height: ScalingUtilities.scaleY(model.height, Controller.gameArenaSize.height, gameArea.height)
            radius: model.radius
            color: model.color
            visible: model.alive
        }
    }

    /* Input handler */
    GameInput {
        id: gameInput
        objectName: "gameInput"
        anchors.fill: parent
    }
}
