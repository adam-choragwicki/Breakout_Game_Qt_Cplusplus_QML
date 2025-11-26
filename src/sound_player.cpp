#include "sound_player.h"
#include "asset_management/asset_repository.h"
#include <QRandomGenerator>
#include <QDebug>

void SoundPlayer::playPaddleCollision()
{
    if (auto* s = AssetRepository::instance().sound("paddle"))
    {
        s->play();
    }
    else
    {
        qCritical() << "Paddle sound missing!";
    }
}

void SoundPlayer::playBrickCollision() const
{
    const auto& sounds = AssetRepository::instance().sounds("brick");

    if (!sounds.empty())
    {
        const uint idx = QRandomGenerator::global()->bounded(sounds.size());
        sounds[idx]->play();
    }
}
