#include "bricks_model.h"

BricksModel::BricksModel(QObject* parent) : QAbstractListModel(parent)
{
}

void BricksModel::setBricksManager(BricksManager* manager)
{
    bricksManager_ = manager;
}

void BricksModel::refresh()
{
    beginResetModel();
    endResetModel();
}

int BricksModel::rowCount(const QModelIndex&) const
{
    return bricksManager_ ? static_cast <int>(bricksManager_->getBricks().size()) : 0;
}

QVariant BricksModel::data(const QModelIndex& index, int role) const
{
    if (!bricksManager_)
    {
        return {};
    }

    const auto& brick = bricksManager_->getBricks().at(index.row());

    switch (role)
    {
        case XRole: return brick.x();
        case YRole: return brick.y();
        case WidthRole: return brick.width();
        case HeightRole: return brick.height();
        case RadiusRole: return brick.radius();
        case ColorRole: return brick.color();
        case AliveRole: return brick.isAlive();
    }

    return {};
}

QHash <int, QByteArray> BricksModel::roleNames() const
{
    return {
        {XRole, "x"}, {YRole, "y"}, {WidthRole, "width"}, {HeightRole, "height"}, {RadiusRole, "radius"}, {ColorRole, "color"}, {AliveRole, "alive"}
    };
}

void BricksModel::brickChanged(const int index)
{
    if (!bricksManager_)
    {
        return;
    }

    QModelIndex idx = createIndex(index, 0);
    emit dataChanged(idx, idx, {AliveRole});
}
