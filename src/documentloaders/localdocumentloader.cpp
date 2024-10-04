#include "localdocumentloader.h"

LocalDocumentLoader::LocalDocumentLoader(const QUrl &path)
    : m_path{path},
    m_doc{path.toEncoded()},
    m_stream{&m_doc},
    _is_valid{false}
{
    if (!m_doc.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QString line = m_stream.readLine();
    QJsonValue meta = _extract_json_from_comment("document-meta", line);
    if (!meta.isObject()) {
        return;
    }

    _is_valid = true;
    m_metainfo = meta.toObject().toVariantMap();
}

bool LocalDocumentLoader::isValid() const
{
    return _is_valid;
}

std::string LocalDocumentLoader::version() const
{
    if (!isValid() || !m_metainfo.contains("version")) {
        return "";
    }

    return m_metainfo["version"].toString().toStdString();
}

std::vector<std::string> LocalDocumentLoader::tags() const
{
    if (!isValid() || !m_metainfo.contains("tags")) {
        return {};
    }

    std::vector<std::string> result;
    for (auto tag : m_metainfo["tags"].toStringList()) {
        result.push_back(tag.toStdString());
    }
    return result;
}

bool LocalDocumentLoader::canFetchMore() const
{
    return isValid() && !m_stream.atEnd();
}

std::vector<std::unique_ptr<AbstractDocumentLoader::Block>> LocalDocumentLoader::fetchMore()
{
    std::vector<std::unique_ptr<Block>> result;
    while (canFetchMore() && result.size() < m_items_to_fetch) {
        std::unique_ptr<Block> block = std::make_unique<Block>();

        QString buf;
        int block_start_pos;
        do {
            block_start_pos = m_stream.pos();
            buf = m_stream.readLine();
        } while (buf.isEmpty() && !m_stream.atEnd());

        QJsonValue meta = _extract_json_from_comment("block-meta", buf);

        if (meta.isObject()) {
            _extract_meta(*block, meta.toObject().toVariantMap());
        } else {
            m_stream.seek(block_start_pos);
        }

        _read_block(*block);

        if (!block->content.empty()) {
            result.push_back(std::move(block));
        }
    }
    return result;
}

void LocalDocumentLoader::setFetchNumber(const int n)
{
    m_items_to_fetch = n;
}

QJsonValue LocalDocumentLoader::_extract_json_from_comment(const QString meta_name, const QString &comment)
{
    if (!(comment.startsWith("<!---") && comment.endsWith("-->"))) {
        return QJsonValue();
    }

    QString buf = comment.chopped(3).remove(0, 5);
    auto decoded = QJsonDocument::fromJson(buf.toUtf8());
    if (decoded.isNull()) {
        return QJsonValue();
    }

    return decoded[meta_name];
}

void LocalDocumentLoader::_extract_meta(Block &block, const QVariantMap &meta_map)
{
    for (const auto& tag : meta_map.value("tags", {}).toStringList()) {
        block.tags.push_back(tag.toStdString());
    }

    block.badge_text = meta_map.value("badge", "").toString().toStdString();
    block.color = meta_map.value("color", "").toString().toStdString();
    block.type = meta_map.value("type", "text").toString().toStdString();
}

void LocalDocumentLoader::_read_block(Block &block)
{
    if (block.type == "video") {
        _read_video(block);
    }

    QString buf;
    int block_start_pos;
    do {
        block_start_pos = m_stream.pos();
        buf = m_stream.readLine();
    } while (buf.isEmpty() && !m_stream.atEnd());

    if (buf.contains("<")) {
        m_stream.seek(block_start_pos);
        _read_html(block);

    } else if (buf.startsWith("```")) {
        m_stream.seek(block_start_pos);
        _read_code(block);

    } else {
        m_stream.seek(block_start_pos);
        _read_text(block);
    }
}

void LocalDocumentLoader::_read_text(Block &block)
{
    QString buf;
    while ((buf = m_stream.readLine()) != "" && !m_stream.atEnd()) {
        block.content.append("\n");
        block.content.append(buf.toStdString());
    }
}

void LocalDocumentLoader::_read_video(Block &block)
{
    QString buf{};
    while ((buf = m_stream.readLine()) == "" && !m_stream.atEnd());
    block.content = m_path.resolved(buf).toEncoded().toStdString();
}

void LocalDocumentLoader::_read_html(Block &block)
{
    QString buf;

    while ((buf = m_stream.readLine()) != "" && !m_stream.atEnd()) {
        block.content.append("\n");
        block.content.append(buf.toStdString());
    }
}

void LocalDocumentLoader::_read_code(Block &block)
{
    block.content.append(m_stream.readLine().toStdString());
    QString buf;
    while (!(buf = m_stream.readLine()).endsWith("```") && !m_stream.atEnd()) {
        block.content.append("\n");
        block.content.append(buf.toStdString());
    }
}
