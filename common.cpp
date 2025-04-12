#include "common.h"
#include <QFile>
#include <QRegularExpression>

void VlcPlayerMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formattedMessage = QString("%1: %2").arg(timestamp, msg);

    fprintf(type == QtInfoMsg ? stdout : stderr, "%s\n", qPrintable(formattedMessage));
    fflush(stdout);
}

void addToFav()
{
  // TODO: add

}
