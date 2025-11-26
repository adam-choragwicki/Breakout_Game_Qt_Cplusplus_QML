#include "brick_layout_library.h"
#include <QDebug>

constexpr qreal percent(const qreal value)
{
    return value / 100.0;
}

std::vector<BrickLayoutEntry> BrickLayoutLibrary::defaultPattern()
{
    std::vector<BrickLayoutEntry> brickLayout;

    // Configuration (normalized to game arena size)
    constexpr int rows = 5;
    constexpr int columns = 5;

    constexpr qreal rowSpacingRelativeToGameArenaHeight = percent(2);
    constexpr qreal columnSpacingRelativeToGameArenaWidth = percent(2);
    constexpr qreal brickFieldHeightRelativeToGameArenaHeight = percent(40);

    // Derived normalized sizes
    constexpr qreal totalRowSpacingRelativeToGameArenaHeight = rowSpacingRelativeToGameArenaHeight * (rows - 1);
    constexpr qreal totalColumnSpacingRelativeToGameArenaWidth = columnSpacingRelativeToGameArenaWidth * (columns - 1);

    constexpr qreal brickWidthRelativeToGameArenaWidth = (1.0 - totalColumnSpacingRelativeToGameArenaWidth) / columns;
    constexpr qreal brickHeightRelativeToGameArenaHeight = (brickFieldHeightRelativeToGameArenaHeight - totalRowSpacingRelativeToGameArenaHeight) / rows;

    if constexpr (constexpr bool ENABLE_DEBUG_BRICK_LAYOUT = false; ENABLE_DEBUG_BRICK_LAYOUT)
    {
        constexpr qreal horizontalCoverage = columns * brickWidthRelativeToGameArenaWidth + totalColumnSpacingRelativeToGameArenaWidth;
        constexpr qreal verticalCoverage = rows * brickHeightRelativeToGameArenaHeight + totalRowSpacingRelativeToGameArenaHeight;

        qDebug().noquote() << "=== Brick Layout Statistics ===\n" << "Columns:" << columns << "  Rows:" << rows << "\n" << "Brick Width:" <<
                brickWidthRelativeToGameArenaWidth * 100.0 << "%  " << "Brick Height:" << brickHeightRelativeToGameArenaHeight * 100.0 << "%\n" <<
                "Column Spacing:" << columnSpacingRelativeToGameArenaWidth * 100.0 << "%  " << "Row Spacing:" << rowSpacingRelativeToGameArenaHeight * 100.0 <<
                "%\n" << "Horizontal coverage:" << horizontalCoverage * 100.0 << "%\n" << "Vertical coverage (of game arena height field):" <<
                verticalCoverage * 100.0 << "%\n" << "Brick field top-to-bottom (relative to game arena size):" << brickFieldHeightRelativeToGameArenaHeight * 100.0
                << "%\n";

        // Sanity checks
        if (horizontalCoverage > 1.001)
            qWarning() << "[Warning] Horizontal coverage exceeds 100%! Layout may overflow.";

        if (horizontalCoverage < 0.99)
            qWarning() << "[Warning] Horizontal coverage < 99%. There may be gaps.";

        if (verticalCoverage > brickFieldHeightRelativeToGameArenaHeight * 1.001)
            qWarning() << "[Warning] Vertical coverage exceeds brick field height!";

        if (verticalCoverage < brickFieldHeightRelativeToGameArenaHeight * 0.99)
            qWarning() << "[Warning] Vertical coverage smaller than field height (visible gaps possible).";
    }

    // Create a pattern
    for (int row = 0; row < rows; ++row)
    {
        QColor rowColor;
        switch (row % 5)
        {
            case 0: rowColor = Qt::red;
                break;
            case 1: rowColor = QColor("orange");
                break;
            case 2: rowColor = Qt::yellow;
                break;
            case 3: rowColor = Qt::green;
                break;
            default: rowColor = Qt::blue;
                break;
        }

        const qreal brickYRelativeToGameArenaHeight = row * (brickHeightRelativeToGameArenaHeight + rowSpacingRelativeToGameArenaHeight);

        for (int column = 0; column < columns; ++column)
        {
            const qreal brickXRelativeToGameArenaWidth = column * (brickWidthRelativeToGameArenaWidth + columnSpacingRelativeToGameArenaWidth);

            brickLayout.push_back({
                brickXRelativeToGameArenaWidth, brickYRelativeToGameArenaHeight, brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight,
                rowColor
            });
        }
    }

    return brickLayout;
}

std::vector<BrickLayoutEntry> BrickLayoutLibrary::classicPattern()
{
    std::vector<BrickLayoutEntry> layout;

    // Configuration (normalized to game arena size)
    constexpr int rows = 6;
    constexpr int columns = 10;

    constexpr qreal rowSpacingRelativeToGameArenaHeight = percent(0.5); // % of game arena height between rows
    constexpr qreal columnSpacingRelativeToGameArenaWidth = percent(0.5); // % of game arena width between columns
    constexpr qreal brickHeightRelativeToGameArenaHeight = percent(5); // brick's height = % of game arena height
    constexpr qreal topOffsetRelativeToGameArenaHeight = percent(10); // bricks starts populating space % from the top of the game arena

    // Derived normalized sizes
    constexpr qreal totalColumnSpacingRelativeToGameArenaWidth = columnSpacingRelativeToGameArenaWidth * (columns - 1);
    constexpr qreal brickWidthRelativeToGameArenaWidth = (1.0 - totalColumnSpacingRelativeToGameArenaWidth) / columns;

    if constexpr (constexpr bool ENABLE_DEBUG_BRICK_LAYOUT = false; ENABLE_DEBUG_BRICK_LAYOUT)
    {
        constexpr qreal horizontalCoverage = columns * brickWidthRelativeToGameArenaWidth + totalColumnSpacingRelativeToGameArenaWidth;
        constexpr qreal verticalCoverage = rows * (brickHeightRelativeToGameArenaHeight + rowSpacingRelativeToGameArenaHeight);

        qDebug().noquote() << "=== Brick Layout Statistics ===\n" << "Columns:" << columns << "  Rows:" << rows << "\n" << "Brick Width:" <<
                brickWidthRelativeToGameArenaWidth * 100.0 << "%  " << "Brick Height:" << brickHeightRelativeToGameArenaHeight * 100.0 << "%\n" <<
                "Column Spacing:" << columnSpacingRelativeToGameArenaWidth * 100.0 << "%  " << "Row Spacing:" << rowSpacingRelativeToGameArenaHeight * 100.0 <<
                "%\n" << "Top offset:" << topOffsetRelativeToGameArenaHeight * 100.0 << "%\n" << "Horizontal coverage:" << horizontalCoverage * 100.0 <<
                "%\n" << "Vertical coverage (rows + spacings):" << verticalCoverage * 100.0 << "%\n";

        if (horizontalCoverage > 1.001)
            qWarning() << "[Warning] Horizontal coverage exceeds 100%! Layout may overflow.";

        if (horizontalCoverage < 0.99)
            qWarning() << "[Warning] Horizontal coverage < 99%. There may be gaps.";
    }

    // Create a pattern
    for (int row = 0; row < rows; ++row)
    {
        const QColor color = QColor::fromHsv((row * 40) % 360, 255, 220);
        const qreal brickYRelativeToGameArenaHeight = topOffsetRelativeToGameArenaHeight + row * (
                                                          brickHeightRelativeToGameArenaHeight + rowSpacingRelativeToGameArenaHeight);

        for (int column = 0; column < columns; ++column)
        {
            const qreal brickXRelativeToGameArenaWidth = column * (brickWidthRelativeToGameArenaWidth + columnSpacingRelativeToGameArenaWidth);

            layout.push_back({
                brickXRelativeToGameArenaWidth, brickYRelativeToGameArenaHeight, brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, color
            });
        }
    }

    return layout;
}

std::vector<BrickLayoutEntry> BrickLayoutLibrary::pyramidPattern()
{
    std::vector<BrickLayoutEntry> layout;

    // Configuration (normalized to game arena size)
    constexpr int pyramidBaseColumns = 9;
    constexpr qreal spacing = percent(0.6); // % of game arena width
    constexpr qreal brickHeightRelativeToGameArenaHeight = percent(5); // brick's height = % of game arena height
    constexpr qreal topOffsetRelativeToGameArenaHeight = percent(10); // bricks starts populating space % from the top of the game arena

    // Derived normalized sizes
    constexpr qreal brickWidthRelativeToGameArenaWidth = (1.0 - spacing * (pyramidBaseColumns - 1)) / pyramidBaseColumns;

    // Create a pyramid pattern
    for (int row = 0; row < 5; ++row)
    {
        const int columns = pyramidBaseColumns - row;
        const qreal totalWidth = columns * brickWidthRelativeToGameArenaWidth + (columns - 1) * spacing;
        const qreal xOffset = (1.0 - totalWidth) / 2.0; // center horizontally
        const qreal brickYRelativeToGameArenaHeight = topOffsetRelativeToGameArenaHeight + row * (brickHeightRelativeToGameArenaHeight + spacing);

        const QColor color = QColor::fromHsv((row * 50) % 360, 200, 230);

        for (int column = 0; column < columns; ++column)
        {
            const qreal brickXRelativeToGameArenaWidth = xOffset + column * (brickWidthRelativeToGameArenaWidth + spacing);
            layout.push_back({
                brickXRelativeToGameArenaWidth, brickYRelativeToGameArenaHeight, brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, color
            });
        }
    }

    return layout;
}

std::vector<BrickLayoutEntry> BrickLayoutLibrary::checkerboardPattern()
{
    std::vector<BrickLayoutEntry> layout;

    // Configuration (normalized to game arena size)
    constexpr int rows = 8;
    constexpr int columns = 10;
    constexpr qreal spacing = percent(0.6); // % of game arena width
    constexpr qreal topOffsetRelativeToGameArenaHeight = percent(8); // bricks starts populating space % from the top of the game arena
    constexpr qreal brickHeightRelativeToGameArenaHeight = percent(4.5); // brick's height = % of game arena height

    // Derived normalized sizes
    constexpr qreal brickWidthRelativeToGameArenaWidth = (1.0 - spacing * (columns - 1)) / columns;

    // Colors
    const QColor colorA(0x1E90FF); // Dodger blue
    const QColor colorB(0xFFD700); // Gold

    // Build checkerboard layout
    for (int row = 0; row < rows; ++row)
    {
        const qreal brickYRelativeToGameArenaHeight = topOffsetRelativeToGameArenaHeight + row * (brickHeightRelativeToGameArenaHeight + spacing);

        for (int column = 0; column < columns; ++column)
        {
            const QColor color = ((row + column) % 2 == 0) ? colorA : colorB;
            const qreal brickXRelativeToGameArenaHeight = column * (brickWidthRelativeToGameArenaWidth + spacing);

            layout.push_back({
                brickXRelativeToGameArenaHeight, brickYRelativeToGameArenaHeight, brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, color
            });
        }
    }

    return layout;
}

std::vector<BrickLayoutEntry> BrickLayoutLibrary::smilePattern()
{
    std::vector<BrickLayoutEntry> layout;

    // Configuration
    constexpr qreal brickWidthRelativeToGameArenaWidth = percent(4);
    constexpr qreal brickHeightRelativeToGameArenaHeight = percent(4);
    constexpr qreal faceCenterX = percent(50);
    constexpr qreal faceCenterY = percent(40);
    constexpr qreal faceRadiusX = percent(25);
    constexpr qreal faceRadiusY = percent(25);

    // Colors
    const auto faceColor = QColor(0xFFF200); // yellow
    const auto eyeColor = QColor("blue");
    const auto mouthColor = QColor(0xE65C00); // orange-red

    // --- FACE OUTLINE (rough circle made of bricks) ---
    for (int deg = 0; deg < 360; deg += 20)
    {
        const qreal rad = qDegreesToRadians(static_cast<qreal>(deg));
        const qreal x = faceCenterX + faceRadiusX * std::cos(rad);
        const qreal y = faceCenterY + faceRadiusY * std::sin(rad);

        layout.push_back({
            x - brickWidthRelativeToGameArenaWidth / 2, y - brickHeightRelativeToGameArenaHeight / 2, brickWidthRelativeToGameArenaWidth,
            brickHeightRelativeToGameArenaHeight, faceColor
        });
    }

    // --- EYES ---
    layout.push_back({
        faceCenterX - percent(10), faceCenterY - percent(10), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, eyeColor
    });

    layout.push_back({
        faceCenterX + percent(6), faceCenterY - percent(10), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, eyeColor
    });

    // --- SMILE (arc, curved upwards) ---
    constexpr qreal smileRadiusX = percent(15);
    constexpr qreal smileRadiusY = percent(10);
    constexpr qreal smileCenterY = faceCenterY + percent(5); // move arc slightly below the center

    // flip the arc: start at 20° and go to 160° to make it curve upwards
    for (int deg = 20; deg <= 160; deg += 10)
    {
        const qreal rad = qDegreesToRadians(static_cast<qreal>(deg));
        const qreal x = faceCenterX + smileRadiusX * std::cos(rad);
        const qreal y = smileCenterY + smileRadiusY * std::sin(rad);

        layout.push_back({
            x - brickWidthRelativeToGameArenaWidth / 2, y - brickHeightRelativeToGameArenaHeight / 2, brickWidthRelativeToGameArenaWidth,
            brickHeightRelativeToGameArenaHeight, mouthColor
        });
    }

    return layout;
}

std::vector<BrickLayoutEntry> BrickLayoutLibrary::manualPattern()
{
    std::vector<BrickLayoutEntry> layout;

    // Configuration (normalized to game arena size)
    constexpr qreal brickWidthRelativeToGameArenaWidth = percent(10); // brick's width = % of game arena width
    constexpr qreal brickHeightRelativeToGameArenaHeight = percent(5); // brick's height = % of game arena height

    // Manually placed bricks
    layout.push_back({percent(10), percent(10), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, Qt::red});

    layout.push_back({percent(30), percent(10), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, Qt::green});

    layout.push_back({percent(50), percent(10), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, Qt::blue});

    layout.push_back({percent(20), percent(25), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, QColor("orange")});

    layout.push_back({percent(40), percent(40), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, QColor("purple")});

    layout.push_back({percent(60), percent(55), brickWidthRelativeToGameArenaWidth, brickHeightRelativeToGameArenaHeight, QColor("cyan")});

    return layout;
}
