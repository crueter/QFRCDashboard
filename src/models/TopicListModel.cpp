#include "TopicListModel.h"

TopicListModel::TopicListModel(TopicStore &store, QObject *parent)
    : QStandardItemModel(parent)
    , m_store(&store)
{
    QHash<int, QByteArray> rez = QStandardItemModel::roleNames();
    rez.insert(TLMRoleTypes::NAME, "name");
    rez.insert(TLMRoleTypes::TYPE, "type");
    rez.insert(TLMRoleTypes::TOPIC, "topic");

    QStandardItemModel::setItemRoleNames(rez);
}

QVariant TopicListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == TLMRoleTypes::NAME) {
        return itemFromIndex(index)->text();
    }

    return QStandardItemModel::data(index, role);
}

void TopicListModel::reload()
{
    fetchMore(QModelIndex());
}

void TopicListModel::add(const QString &toAdd)
{
    if (toAdd.isEmpty()) return;

    QStringList split = toAdd.split('/');
    if (split.at(0).isEmpty()) split.remove(0);

    QStandardItem *parentItem = invisibleRootItem();

    for (const QString &sub : split) {
        bool isLast = sub == split.last();

        auto results = findItems(sub, Qt::MatchRecursive | Qt::MatchExactly | Qt::MatchWrap);

        if (results.isEmpty()) {
            QStandardItem *item = new QStandardItem(sub);

            if (isLast) {
                item->setData(toAdd, TLMRoleTypes::TOPIC);
                item->setData(m_store->typeString(toAdd), TYPE);
            } else {
                item->setData("theseNuts", TLMRoleTypes::TOPIC);
                item->setData("", TYPE);
            }

            parentItem->appendRow(item);
            parentItem = item;
        } else {
            for (QStandardItem *item : results) {
                if (item->parent() == nullptr || item->parent()->text() == parentItem->text()) {
                    parentItem = item;
                }
            }
        }
    }
}

void TopicListModel::remove(const QString &toRemove)
{
    if (toRemove.isEmpty()) return;

    QStringList split = toRemove.split('/');
    if (split.at(0).isEmpty()) split.remove(0);

    QStandardItem *parentItem = invisibleRootItem();
    for (const QString &sub : split) {
        auto results = findItems(sub, Qt::MatchRecursive | Qt::MatchExactly | Qt::MatchWrap);

        if (results.isEmpty()) return;
        else {
            for (QStandardItem *item : results) {
                if (item == nullptr) continue;

                if (item->parent() == nullptr || item->parent() == parentItem) {
                    if (!item->hasChildren()) {
                        parentItem->removeRow(item->row());
                        if (!parentItem->hasChildren()) {
                            if (parentItem->parent() == nullptr) {
                                removeRow(parentItem->row());
                            } else {
                                parentItem->parent()->removeRow(parentItem->row());
                            }
                        }
                    }

                    parentItem = item;

                }
            }
        }
    }
}
