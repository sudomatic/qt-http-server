
#include "QtHttpReply.h"

QtHttpReply::QtHttpReply (QObject * parent)
    : QObject      (parent)
    , m_statusCode (Ok)
    , m_data       (QByteArray ())
{

}

int QtHttpReply::getRawDataSize () const {
    return m_data.size ();
}

QtHttpReply::StatusCode QtHttpReply::getStatusCode () const {
    return m_statusCode;
}

QByteArray QtHttpReply::getResponseData () const {
    return m_data;
}

QHash<QByteArray, QByteArray> QtHttpReply::getHeaders () const {
    return m_headersHash;
}

void QtHttpReply::setStatusCode (QtHttpReply::StatusCode statusCode) {
    m_statusCode = statusCode;
}

void QtHttpReply::appendRawData (QByteArray data) {
    m_data.append (data);
}

void QtHttpReply::addHeader (QByteArray header, QByteArray value) {
    QByteArray key = header.trimmed ();
    if (!key.isEmpty ()) {
        m_headersHash.insert (key, value);
    }
}
