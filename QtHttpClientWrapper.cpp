
#include "QtHttpClientWrapper.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpServer.h"
#include "QtHttpHeader.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QStringBuilder>

#define CRLF  QByteArrayLiteral ("\r\n")
#define SPACE char (' ')
#define COLON char (':')

inline static const QString hashFromPointerAddress (void * pointer) {
    return QString::fromLocal8Bit (
                QCryptographicHash::hash (
                    QByteArray::number ((quint64) (pointer)),
                    QCryptographicHash::Md5
                    ).toHex ()
                );
}

QtHttpClientWrapper::QtHttpClientWrapper (QTcpSocket * sock, QtHttpServer * parent)
    : QObject          (parent)
    , m_guid           (hashFromPointerAddress (sock))
    , m_parsingStatus  (AwaitingRequest)
    , m_sockClient     (sock)
    , m_currentRequest (Q_NULLPTR)
    , m_serverHandle   (parent)
{
    connect (m_sockClient, &QTcpSocket::readyRead, this, &QtHttpClientWrapper::onClientDataReceived);
}

QString QtHttpClientWrapper::getGuid (void) const {
    return m_guid;
}

void QtHttpClientWrapper::onClientDataReceived () {
    if (m_sockClient) {
        while (m_sockClient->canReadLine ()) {
            QByteArray line = m_sockClient->readLine ().trimmed ();
            switch (m_parsingStatus) { // handle parsing steps
                case AwaitingRequest: { // "command url version" × 1
                    QString str = QString::fromUtf8 (line);
                    QStringList parts = str.split (SPACE, QString::SkipEmptyParts);
                    if (parts.size () == 3) {
                        QString command = parts.at (0);
                        QString url     = parts.at (1);
                        QString version = parts.at (2);
                        if (version == m_serverHandle->getHttpVersion ()) {
                            //qDebug () << "Debug : HTTP"
                            //          << "command :" << command
                            //          << "url :"     << url
                            //          << "version :" << version;
                            m_currentRequest = new QtHttpRequest (m_serverHandle);
                            m_currentRequest->setUrl     (QUrl (url));
                            m_currentRequest->setCommand (command);
                            m_parsingStatus = AwaitingHeaders;
                        }
                        else {
                            m_parsingStatus = ParsingError;
                            //qWarning () << "Error : unhandled HTTP version :" << version;
                        }
                    }
                    else {
                        m_parsingStatus = ParsingError;
                        //qWarning () << "Error : incorrect HTTP command line :" << line;
                    }
                    break;
                }
                case AwaitingHeaders: { // "header: value" × N (until empty line)
                    if (!line.isEmpty ()) { // parse headers
                        int pos = line.indexOf (COLON);
                        if (pos > 0) {
                            QByteArray header = line.left (pos).trimmed ();
                            QByteArray value  = line.mid  (pos +1).trimmed ();
                            //qDebug () << "Debug : HTTP"
                            //          << "header :" << header
                            //          << "value :"  << value;
                            m_currentRequest->addHeader (header, value);
                            if (header == QtHttpHeader::ContentLength) {
                                int  len = -1;
                                bool ok  = false;
                                len = value.toInt (&ok, 10);
                                if (ok) {
                                    m_currentRequest->addHeader (QtHttpHeader::ContentLength, QByteArray::number (len));
                                }
                            }
                        }
                        else {
                            m_parsingStatus = ParsingError;
                            qWarning () << "Error : incorrect HTTP headers line :" << line;
                        }
                    }
                    else { // end of headers
                        //qDebug () << "Debug : HTTP end of headers";
                        if (m_currentRequest->getHeader (QtHttpHeader::ContentLength).toInt () > 0) {
                            m_parsingStatus = AwaitingContent;
                        }
                        else {
                            m_parsingStatus = RequestParsed;
                        }
                    }
                    break;
                }
                case AwaitingContent: { // raw data × N (until EOF ??)
                    m_currentRequest->appendRawData (line);
                    //qDebug () << "Debug : HTTP"
                    //          << "content :" << m_currentRequest->getRawData ().toHex ()
                    //          << "size :"    << m_currentRequest->getRawData ().size  ();
                    if (m_currentRequest->getRawDataSize () == m_currentRequest->getHeader (QtHttpHeader::ContentLength).toInt ()) {
                        //qDebug () << "Debug : HTTP end of content";
                        m_parsingStatus = RequestParsed;
                    }
                    break;
                }
                default: { break; }
            }
            switch (m_parsingStatus) { // handle parsing status end/error
                case RequestParsed: { // a valid request has ben fully parsed
                    QtHttpReply reply (m_serverHandle);
                    connect (&reply, &QtHttpReply::requestSendHeaders,
                             this, &QtHttpClientWrapper::onReplySendHeadersRequested);
                    connect (&reply, &QtHttpReply::requestSendData,
                             this, &QtHttpClientWrapper::onReplySendDataRequested);
                    emit m_serverHandle->requestNeedsReply (m_currentRequest, &reply); // allow app to handle request
                    m_parsingStatus = sendReplyToClient (&reply);
                    break;
                }
                case ParsingError: { // there was an error durin one of parsing steps
                    m_sockClient->readAll (); // clear remaining buffer to ignore content
                    QtHttpReply reply (m_serverHandle);
                    reply.setStatusCode (QtHttpReply::BadRequest);
                    reply.appendRawData (QByteArrayLiteral ("<h1>Bad Request (HTTP parsing error) !</h1>"));
                    reply.appendRawData (CRLF);
                    m_parsingStatus = sendReplyToClient (&reply);
                    break;
                }
                default: { break; }
            }
        }
    }
}

void QtHttpClientWrapper::onReplySendHeadersRequested (void) {
    QtHttpReply * reply = qobject_cast<QtHttpReply *> (sender ());
    if (reply != Q_NULLPTR) {
        QByteArray data;
        // HTTP Version + Status Code + Status Msg
        data.append (m_serverHandle->getHttpVersion ());
        data.append (SPACE);
        data.append (QByteArray::number (reply->getStatusCode ()));
        data.append (SPACE);
        data.append (QtHttpReply::getStatusTextForCode (reply->getStatusCode ()));
        data.append (CRLF);
        if (reply->useChunked ()) {
            reply->addHeader (QtHttpHeader::TransferEncoding, "chunked");
        }
        else {
            reply->addHeader (QtHttpHeader::ContentLength, QByteArray::number (reply->getRawDataSize ()));
        }
        // Header name: header value
        foreach (QByteArray header, reply->getHeadersList ()) {
            data.append (header);
            data.append (COLON);
            data.append (SPACE);
            data.append (reply->getHeader (header));
            data.append (CRLF);
        }
        // empty line
        data.append (CRLF);
        m_sockClient->write (data);
        m_sockClient->flush ();
    }
}

void QtHttpClientWrapper::onReplySendDataRequested (void) {
    QtHttpReply * reply = qobject_cast<QtHttpReply *> (sender ());
    if (reply != Q_NULLPTR) {
        // content raw data
        QByteArray data = reply->getRawData ();
        if (reply->useChunked ()) {
            data.prepend (QByteArray::number (data.size (), 16) % CRLF);
            data.append (CRLF);
            reply->resetRawData ();
        }
        // write to socket
        m_sockClient->write (data);
        m_sockClient->flush ();
    }
}

QtHttpClientWrapper::ParsingStatus QtHttpClientWrapper::sendReplyToClient (QtHttpReply * reply) {
    if (!reply->useChunked ()) {
        reply->appendRawData (CRLF);
        // force send all headers
        reply->requestSendHeaders ();
        // force send all data
        reply->requestSendData ();
    }
    else {
        // last chunk
        QByteArray data = ("0" % CRLF % CRLF);
        data.append (reply->getRawData ());
        m_sockClient->write (data);
        m_sockClient->flush ();
    }
    if (m_currentRequest) {
        if (m_currentRequest->getHeader (QtHttpHeader::Connection).toLower () == QByteArrayLiteral ("close")) {
            // must close connection after this request
            m_sockClient->close ();
        }
        m_currentRequest->deleteLater ();
        m_currentRequest = Q_NULLPTR;
    }
    return AwaitingRequest;
}
