#pragma once

#include <QUrl>

class AssetLoader
{
public:
    static bool loadAssets();


private:
    static bool loadSounds();
    static void loadPaddleCollisionSound();
    static void loadBrickCollisionSounds();
    static bool checkIfResourceExists(const QUrl& url);
};
