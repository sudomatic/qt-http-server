
#include "QtHttpRequest.h"
#include "QtHttpHeader.h"

QtHttpRequest::QtHttpRequest (QObject * parent)
    : QObject         (parent)
    , m_url           (QUrl ())
    , m_command       (QString ())
    , m_data          (QByteArray ())
{
    m_headersHash.insert (QtHttpHeader::ContentLength, QByteArrayLiteral ("0"));
    m_headersHash.insert (QtHttpHeader::Connection,    QByteArrayLiteral ("Keep-Alive"));
}

QUrl QtHttpRequest::getUrl () const {
    return m_url;
}

QByteArray QtHttpRequest::getRawData () const {
    return m_data;
}

QList<QByteArray> QtHttpRequest::getHeadersList () const {
    return m_headersHash.keys ();
}

QByteArray QtHttpRequest::getHeader (QByteArray header) const {
    return m_headersHash.value (header, QByteArray ());
}

void QtHttpRequest::setUrl (QUrl url) {
    m_url = url;
}

void QtHttpRequest::setCommand (QString command) {
    m_command = command;
}

void QtHttpRequest::addHeader (QByteArray header, QByteArray value) {
    QByteArray key = header.trimmed ();
    if (!key.isEmpty ()) {
        m_headersHash.insert (key, value);
    }
}

void QtHttpRequest::appendRawData (QByteArray data) {
    m_data.append (data);
}
