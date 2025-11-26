#pragma once

#include <vector>
#include <memory>
#include <QSoundEffect>
#include <QString>
#include <unordered_map>
#include <QDebug>

class AssetRepository
{
public:
    static AssetRepository& instance()
    {
        static AssetRepository repo;
        return repo;
    }

    // Store a single sound. Key is a name, value is a vector.
    void storeSound(const QString& name, std::unique_ptr <QSoundEffect> sound)
    {
        sounds_[name].push_back(std::move(sound));
    }

    // Retrieve all sounds for a logical name
    [[nodiscard]] const std::vector <std::unique_ptr <QSoundEffect> >& sounds(const QString& name) const
    {
        static const std::vector <std::unique_ptr <QSoundEffect> > empty;
        auto it = sounds_.find(name);
        if (it != sounds_.end())
        {
            return it->second;
        }
        return empty;
    }

    // Single sound convenience helper (first one)
    [[nodiscard]] QSoundEffect* sound(const QString& name) const
    {
        auto it = sounds_.find(name);
        if (it != sounds_.end() && !it->second.empty())
        {
            return it->second[0].get();
        }
        return nullptr;
    }

private:
    std::unordered_map <QString, std::vector <std::unique_ptr <QSoundEffect> > > sounds_{};
};
