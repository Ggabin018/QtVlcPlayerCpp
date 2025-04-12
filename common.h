#pragma once

#include <QDateTime>
#include <QLoggingCategory>

void VlcPlayerMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void addToFav();
