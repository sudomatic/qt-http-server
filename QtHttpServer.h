#ifndef QTHTTPSERVER_H
#define QTHTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

class QtHttpRequest;
class QtHttpReply;
class QtHttpClientWrapper;
class QtHttpServer : public QObject {
    Q_OBJECT

public:
    explicit QtHttpServer     (QObject * parent = NULL);
    virtual ~QtHttpServer     ();

    const QString getServerName () const;

    static const QString getHttpVersion ();

public slots:
    void start                (quint16 port = 0);
    void stop                 ();
    void setServerName        (QString serverName);

signals:
    void started              (quint16 port);
    void stopped              ();
    void error                (QString msg);
    void clientConnected      (QString guid);
    void clientDisconnected   (QString guid);
    void requestNeedsReply    (QtHttpRequest * request,
                               QtHttpReply   * reply);

private slots:
    void onClientConnected    ();
    void onClientDisconnected ();

private:
    QString                                    m_serverName;
    QTcpServer                              *  m_sockServer;
    QHash<QTcpSocket *, QtHttpClientWrapper *> m_socksClientsHash;

    static const QString s_httpVersion;
};

#endif // QTHTTPSERVER_H
