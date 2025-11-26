#include "asset_loader.h"
#include "asset_repository.h"
#include <QSoundEffect>
#include <QFile>
#include <QDebug>

bool AssetLoader::loadAssets()
{
    return loadSounds();
}

bool AssetLoader::loadSounds()
{
    loadPaddleCollisionSound();
    loadBrickCollisionSounds();

    return true;
}

void AssetLoader::loadPaddleCollisionSound()
{
    const QUrl paddleCollisionSoundUrl("qrc:/sounds/paddle_collision.wav");
    checkIfResourceExists(paddleCollisionSoundUrl);

    auto paddleCollisionSoundEffect = std::make_unique <QSoundEffect>();
    paddleCollisionSoundEffect->setSource(paddleCollisionSoundUrl);
    paddleCollisionSoundEffect->setVolume(0.5);
    AssetRepository::instance().storeSound("paddle", std::move(paddleCollisionSoundEffect));
}

void AssetLoader::loadBrickCollisionSounds()
{
    for (int i = 0; i < 3; ++i)
    {
        const QString path = QString("qrc:/sounds/brick_collision_%1.wav").arg(i + 1);
        const QUrl url(path);

        checkIfResourceExists(url);

        auto brickCollisionSoundEffect = std::make_unique <QSoundEffect>();
        brickCollisionSoundEffect->setSource(QUrl(QString("qrc:/sounds/brick_collision_%1.wav").arg(i + 1)));
        brickCollisionSoundEffect->setVolume(0.5);

        AssetRepository::instance().storeSound("brick", std::move(brickCollisionSoundEffect));
    }
}

bool AssetLoader::checkIfResourceExists(const QUrl& url)
{
    const QString resourcePath = url.toString().mid(3); // strip "qrc:" prefix

    if (!QFile::exists(resourcePath))
    {
        throw std::runtime_error("QRC sound resource does not exist: " + url.toString().toStdString());
    }

    return true;
}
