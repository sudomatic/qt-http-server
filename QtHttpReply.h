#ifndef QTHTTPREPLY_H
#define QTHTTPREPLY_H

#include <QObject>

class QtHttpReply : public QObject {
    Q_OBJECT

public:
    explicit QtHttpReply (QObject * parent = NULL);

    QByteArray getResponseData () const;

signals:

public slots:
    void appendRawData (QByteArray data);

private:
    QByteArray m_data;
};

#endif // QTHTTPREPLY_H
