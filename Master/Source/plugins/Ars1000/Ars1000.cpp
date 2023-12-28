#include "Ars1000.h"

#include <QDataStream>

static const quint32 ARS1000_MAGIC = 0xDEADBEEF;

struct PacketHeader {
    quint32 magic;
    quint32 command;
    quint32 size;
};

void Ars1000::onConnected()
{

}

void Ars1000::onDataArrived()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    Link &link = links[socket->property("Link").toInt()];
    QDataStream &s = *(link.stream);
    while (!s.atEnd()) {
        s.startTransaction();

        PacketHeader header;
        s >> header.magic >> header.command >> header.size;
        if (s.status() != QDataStream::Ok) {
            s.rollbackTransaction();
            return;
        }
        if (header.magic != ARS1000_MAGIC) {
            s.skipRawData(1);
            s.commitTransaction();
            continue;
        }

        QByteArray data(header.size, Qt::Uninitialized);
        s >> data;
//        s.readRawData(data.data(), head.lSize);
        if (s.status() != QDataStream::Ok) {
            s.rollbackTransaction();
            return;
        }
        consumePacket(header.command, data);
    }
}

void Ars1000::sendRawPacket(int command, Ars1000::LinkName link, const QByteArray &data)
{
    Q_UNUSED(command);
    Q_UNUSED(link);
    Q_UNUSED(data);
}

void Ars1000::consumePacket(int command,const QByteArray &data)
{
    Q_UNUSED(command);
    Q_UNUSED(data);
//    switch (command) {
//    case CMD_CURRENT_CHANNEL:       break;
//    case CMD_SCAN_STATUS:           break;
//    case CMD_SIMULATE_SET:          break;
//    case CMD_WORKMODE:              break;
//    case CMD_ALL_PARAM_SET:         break;
//    case CMD_RESET:                 break;
//    case CMD_ALL_PARAM_REQ:         break;
//    case CMD_ASCAN:                 break;
//    case CMD_VERSION_REQ:           break;
//    case CMD_WORK_MODE_SET:         break;
//    case CMD_TCG_SWITCH:            break;
//    case CMD_ASCAN_DATA_STORE:      break;
//    case CMD_ASCAN_STOP:            break;
//    case CMD_TEST_START_FLAG:       break;
//    case CMD_CODER_MARK:            break;
//    case CMD_SET_PRF:               break;
//    case CMD_ASCAN_RET_WAVE:        break;
//    case CMD_CSCAN_DATA:            break;
//    case CMD_ASCAN_RET_DATA_STORE:  break;
//    case CMD_SET_PARAM_RET:         break;
//    case CMD_LOWER_PLX_REQ:         break;
//    case CMD_EYES_STATUS_REQ:       break;
//    case CMD_LOWER_PLX_ANSWER:      break;
//    case CMD_EYES_STATUS_ANSWER:    break;
//    case CMD_HEART:                 break;
//    case CMD_FLAW_OR_BOTTOM:        break;
//    }
}
