#ifndef QTHTTPREQUEST_H
#define QTHTTPREQUEST_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QUrl>

class QtHttpRequest : public QObject {
    Q_OBJECT

public:
    explicit QtHttpRequest (QObject * parent = NULL);

    int        getContentLength () const;
    bool       getKeepAlive     () const;
    QUrl       getUrl           () const;
    QByteArray getRawData       () const;
    QByteArray getHeader        (QByteArray header) const;

signals:

public slots:
    void setContentLength (int len);
    void setKeepAlive     (bool keepAlive);
    void setUrl           (QUrl url);
    void setCommand       (QString command);
    void addHeader        (QByteArray header, QByteArray value);
    void appendRawData    (QByteArray data);

private:
    int                           m_contentLength;
    bool                          m_keepAlive;
    QUrl                          m_url;
    QString                       m_command;
    QByteArray                    m_data;
    QHash<QByteArray, QByteArray> m_headersHash;
};

#endif // QTHTTPREQUEST_H
