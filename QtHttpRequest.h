#ifndef QTHTTPREQUEST_H
#define QTHTTPREQUEST_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QUrl>

class QtHttpServer;
class QtHttpRequest : public QObject {
    Q_OBJECT

public:
    explicit QtHttpRequest (QtHttpServer * parent);

    int               getRawDataSize   () const;
    QUrl              getUrl           () const;
    QByteArray        getRawData       () const;
    QList<QByteArray> getHeadersList   () const;

    QByteArray getHeader (QByteArray header) const;

public slots:
    void setUrl           (QUrl url);
    void setCommand       (QString command);
    void addHeader        (QByteArray header, QByteArray value);
    void appendRawData    (QByteArray data);

private:
    QUrl                          m_url;
    QString                       m_command;
    QByteArray                    m_data;
    QtHttpServer                * m_serverHandle;
    QHash<QByteArray, QByteArray> m_headersHash;
};

#endif // QTHTTPREQUEST_H
