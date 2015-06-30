#ifndef QTHTTPCLIENTWRAPPER_H
#define QTHTTPCLIENTWRAPPER_H

#include <QObject>
#include <QTcpSocket>

class QtHttpRequest;
class QtHttpReply;
class QtHttpServer;
class QtHttpClientWrapper : public QObject {
    Q_OBJECT

public:
    explicit QtHttpClientWrapper (QTcpSocket * sock, QtHttpServer * parent);

    QString getGuid () const;

    enum ParsingStatus {
        ParsingError    = -1,
        AwaitingRequest =  0,
        AwaitingHeaders =  1,
        AwaitingContent =  2,
        RequestParsed   =  3
    };

private slots:
    void onClientDataReceived ();

protected:
    ParsingStatus sendReplyToClient (QtHttpReply * reply);

protected slots:
    void onReplySendHeadersRequested ();
    void onReplySendDataRequested    ();

private:
    QString         m_guid;
    ParsingStatus   m_parsingStatus;
    QTcpSocket    * m_sockClient;
    QtHttpRequest * m_currentRequest;
    QtHttpServer  * m_serverHandle;
};

#endif // QTHTTPCLIENTWRAPPER_H
