#include "QtHttpReply.h"

QtHttpReply::QtHttpReply (QObject * parent)
    : QObject (parent)
{

}

QByteArray QtHttpReply::getResponseData () const {
    return m_data;
}

void QtHttpReply::appendRawData (QByteArray data) {
    m_data.append (data);
}
