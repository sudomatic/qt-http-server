
#include "QtHttpRequest.h"

QtHttpRequest::QtHttpRequest (QObject * parent)
    : QObject         (parent)
    , m_contentLength (-1)
    , m_keepAlive     (true)
    , m_url           (QUrl ())
    , m_command       (QString ())
    , m_data          (QByteArray ())
{

}

int QtHttpRequest::getContentLength () const {
    return m_contentLength;
}

bool QtHttpRequest::getKeepAlive () const {
    return m_keepAlive;
}

QUrl QtHttpRequest::getUrl () const {
    return m_url;
}

QByteArray QtHttpRequest::getRawData () const {
    return m_data;
}

QByteArray QtHttpRequest::getHeader (QByteArray header) const {
    return m_headersHash.value (header, QByteArray ());
}

void QtHttpRequest::setContentLength (int len) {
    m_contentLength = len;
}

void QtHttpRequest::setKeepAlive (bool keepAlive) {
    m_keepAlive = keepAlive;
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
