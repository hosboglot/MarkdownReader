#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <documentloaders/localdocumentloader.h>

#include <QAbstractListModel>
#include <QString>
#include <QStringList>
#include <QColor>

#include <memory>

class DocumentModel : public QAbstractListModel
{
    Q_OBJECT

public:
    struct Block {
        QString content{""};
        QStringList tags{};
        QString badge_text{""};
        QColor color{""};
        QString type{"text"};

        bool tags_visible{false};
    };
    enum DocumentRoles {
        ContentRole = Qt::UserRole + 1,
        TagsRole,
        BadgeRole,
        ColorRole,
        TypeRole,
        TagsVisibleRole
    };

    explicit DocumentModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setLocalSource(QUrl &path);
private:

    std::unique_ptr<AbstractDocumentLoader> m_loader;
    std::vector<Block> m_container;
};

#endif // DOCUMENTMODEL_H
