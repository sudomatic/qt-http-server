
#include "QtHttpReply.h"
#include "QtHttpHeader.h"
#include "QtHttpServer.h"

#include <QDateTime>

QtHttpReply::QtHttpReply (QtHttpServer * parent)
    : QObject        (parent)
    , m_statusCode   (Ok)
    , m_data         (QByteArray ())
    , m_serverHandle (parent)
{
    // set some additional headers
    addHeader (QtHttpHeader::Date,   QDateTime::currentDateTimeUtc ().toString ("ddd, dd MMM yyyy hh:mm:ss t").toLatin1 ());
    addHeader (QtHttpHeader::Server, m_serverHandle->getServerName ().toUtf8 ());
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

QList<QByteArray> QtHttpReply::getHeadersList () const {
    return m_headersHash.keys ();
}

QByteArray QtHttpReply::getHeader (QByteArray header) const {
    return m_headersHash.value (header, QByteArray ());
}

const QByteArray QtHttpReply::getStatusTextForCode (QtHttpReply::StatusCode statusCode) {
    switch (statusCode) {
        case Ok:         return QByteArrayLiteral ("OK.");
        case BadRequest: return QByteArrayLiteral ("Bad request !");
        case Forbidden:  return QByteArrayLiteral ("Forbidden !");
        case NotFound:   return QByteArrayLiteral ("Not found !");
        default:         return QByteArrayLiteral ("");
    }
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
