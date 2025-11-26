.pragma library

function scaleX(value, logicalWidth, renderWidth)
{
    return value * renderWidth / logicalWidth;
}

function scaleY(value, logicalHeight, renderHeight)
{
    return value * renderHeight / logicalHeight;
}
