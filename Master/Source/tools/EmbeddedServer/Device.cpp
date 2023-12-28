#include "Device.h"
#include "Client.h"
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QTcpSocket>

static Q_LOGGING_CATEGORY(debug, "server.device")

static size_t fpgaPhysicalSize = 0x400000;

static const size_t maxSocketBuffer = 4 * 1024 * 1024;
static quint16 monitorFlag = 1;

Device::Device(qint64 baseAddr, QObject *parent) :
    QObject(parent)
{
    qCDebug(debug, "Mapping FPGA memory space at physical address %llx, size %x", baseAddr, fpgaPhysicalSize);
    base = mapPhysical(baseAddr, fpgaPhysicalSize);
    qCDebug(debug, "Mapped FPGA memory space at virtual address %p", base);

    if (base == nullptr) {
        qCDebug(debug, "Hardware unavailable.");
        return;
    }

    qCDebug(debug, "Initializing FPGA at %p", base);

    quint16 hardwareVersion = ioread<quint16>(0x000024);
    if (hardwareVersion < 0x500 || hardwareVersion > 0x600) {
        qCDebug(debug, "Invalid hardware version %x", hardwareVersion);
        return;
    }
    qCDebug(debug, "Hardware version is %x", hardwareVersion);

    hardwareValid = true;

    bool isCompressed = (ioread<quint16>(0x000026) & 0x1) != 0;
    if (isCompressed) {
        qCDebug(debug, "Buffer is compressed");
    } else {
        qCDebug(debug, "Buffer is NOT compressed");
    }

    qCDebug(debug, "Length of monitor data is %d", sizeof(monitor));

    for (int channel = 0; channel < 2; ++channel) {
        off_t channelOffset = 0x040000 * channel;
        Section M;
        M.sectionCount =    1;
        M.sectionSize =     sizeof(monitor);
        M.entry[0] =        reinterpret_cast<quint16 *>(&monitor);
        M.bufferIndex =     &monitorFlag;
        M.reset =           reinterpret_cast<quint16 *>(base + 0x060082 + channelOffset);
        M.flag =            reinterpret_cast<quint16 *>(base + 0x060088 + channelOffset);
        M.entry[1] =        reinterpret_cast<quint16 *>(base + 0x06004A + channelOffset);
        M.entry[2] =        reinterpret_cast<quint16 *>(base + 0x060C00 + channelOffset);
        M.update =          &Device::updateMonitor;
        M.updateInterval =  1000 / 50;

        Section C;
        C.sectionCount =    128;
        C.sectionSize =     64;
        C.flag =            reinterpret_cast<quint16 *>(base + 0x06008A + channelOffset);
        C.bufferIndex =     reinterpret_cast<quint16 *>(base + 0x068000 + channelOffset);
        C.reset =           reinterpret_cast<quint16 *>(base + 0x060086 + channelOffset);
        C.entry[0] =        reinterpret_cast<quint16 *>(base + 0x06A000 + channelOffset);
        C.entry[1] =        reinterpret_cast<quint16 *>(base + 0x06C000 + channelOffset);
        C.update =          nullptr;
        C.updateInterval =  0;

        Section A;
        A.sectionCount =    isCompressed ? 60 : 30;
        A.sectionSize =     isCompressed ? (512 + 64) : (1024 + 64);
        A.flag =            reinterpret_cast<quint16 *>(base + 0x06008C + channelOffset);
        A.bufferIndex =     reinterpret_cast<quint16 *>(base + 0x370000 + channelOffset);
        A.reset =           reinterpret_cast<quint16 *>(base + 0x06008E + channelOffset);
        A.entry[0] =        reinterpret_cast<quint16 *>(base + 0x170000 + channelOffset);
        A.entry[1] =        reinterpret_cast<quint16 *>(base + 0x270000 + channelOffset);
        A.update =          nullptr;
        A.updateInterval =  0;
        Channel c = {M, C, A};
        channels.push_back(c);
    }

    reset();
}

Device::~Device()
{
}

QList<Device *> Device::enumerate()
{
    QDir devicesRoot("/sys/bus/platform/devices");
    QList<Device *> devices;
    for (const QFileInfo &deviceDirFile: devicesRoot.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        if (deviceDirFile.isDir()) {
            QDir deviceDir(deviceDirFile.absoluteFilePath());
            if (!deviceDir.exists("modalias")) {
                continue;
            }
            QFile file(deviceDir.filePath("modalias"));
            if (!file.open(QIODevice::ReadOnly)) {
                continue;
            }
            if (file.readAll().contains("allrising,ars2000")) {
                qint64 offset = deviceDirFile.baseName().toLongLong(nullptr, 16);
                qCDebug(debug) << "Probed" << QString::number(offset, 16);
                Device *dev = new Device(offset);
                if (!dev->hardwareValid) {
                    delete dev;
                } else {
                    devices << dev;
                }
            }
        }
    }
    return devices;
}

uchar *Device::mapPhysical(qint64 physicalAddr, qint64 size)
{
    QFile *file = new QFile("/dev/mem", this);
    if (!file->open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
        qCWarning(debug) << "Failed opening mem:" << file->errorString();
        delete file;
        return 0;
    }
    // Runtime warning about "mapping beyond its size" will occur when mmap'ing.
    // Since /dev/mem will have a 0 size, this is intended.
    uchar *virtualAddr = file->map(physicalAddr, size);
    if (!virtualAddr) {
        qCCritical(debug) << "Failed mapping mem:" << file->errorString();
        return 0;
    }
    return virtualAddr;
}

void Device::reset()
{
    for (Channel &channel: channels) {
        for (Section &section: channel) {
            *section.reset = 1;
            *section.reset = 0;
        }
    }
}

struct FpgaBufferHeader
{
    quint32 sectionSize;
    quint32 sectionCount;
    quint16 channel;
    quint16 levelOfService;
    quint16 timeMultiplier;
};

struct CSCAN_ARS200FPGA_CH_FEATURE_REG {
    quint16 wGate1SyncTime;
    quint16 wGate1MPFirstTime;
    quint16 wGate1MPTime;
    quint16 wGate1MPAmp	;
    quint16 wGate1FPFirstTime;
    quint16 wGate1FPTime;
    quint16 wGate1FPAmp;
    quint16 wGate2SyncTime;
    quint16 wGate2MPFirstTime;
    quint16 wGate2MPTime;
    quint16 wGate2MPAmp	;
    quint16 wGate2FPFirstTime;
    quint16 wGate2FPTime;
    quint16 wGate2FPAmp;
    quint16 wGate3SyncTime;
    quint16 wGate3MPFirstTime;
    quint16 wGate3MPTime;
    quint16 wGate3MPAmp	;
    quint16 wGate3FPFirstTime;
    quint16 wGate3FPTime;
    quint16 wGate3FPAmp;
    quint16 wGate4SyncTime;
    quint16 wGate4MPFirstTime;
    quint16 wGate4MPTime;
    quint16 wGate4MPAmp	;
    quint16 wGate4FPFirstTime;
    quint16 wGate4FPTime;
    quint16 wGate4FPAmp;
};

struct Encoders
{
    quint16 wXLow16;
    quint16 wXHigh16;
    quint16 wYLow16;
    quint16 wYHigh16;
    quint16 wZLow16;
    quint16 wZHigh16;
    quint16 wW1Low16;
    quint16 wW1High16;
    quint16 wALow16;
    quint16 wAHigh16;
    quint16 wBLow16;
    quint16 wBHigh16;
    quint16 wW2Low16;
    quint16 wW2High16;
};

void Device::updateMonitor(unsigned int channelIndex)
{
    Channel &channel = channels[channelIndex];
    Section &section = channel[0];

    memcpy(monitor.wave, const_cast<const quint16 *>(section.entry[2]), sizeof(monitor.wave));
    monitor.wave[0] = monitor.wave[1];

    Encoders *cscan_code_reg = reinterpret_cast<Encoders *>(base + 0x30);
    monitor.nXLow16 = cscan_code_reg->wXLow16;
    monitor.nXHigh16 = cscan_code_reg->wXHigh16;
    monitor.nYLow16 = cscan_code_reg->wYLow16;
    monitor.nYHigh16 = cscan_code_reg->wYHigh16;
    monitor.nZLow16 = cscan_code_reg->wZLow16;
    monitor.nZHigh16 = cscan_code_reg->wZHigh16;
    monitor.nW1Low16 = cscan_code_reg->wW1Low16;
    monitor.nW1High16 = cscan_code_reg->wW1High16;
    monitor.nALow16 = cscan_code_reg->wALow16;
    monitor.nAHigh16 = cscan_code_reg->wAHigh16;
    monitor.nBLow16 = cscan_code_reg->wBLow16;
    monitor.nBHigh16 = cscan_code_reg->wBHigh16;
    monitor.nW2Low16 = cscan_code_reg->wW2Low16;
    monitor.nW2High16 = cscan_code_reg->wW2High16;

    volatile CSCAN_ARS200FPGA_CH_FEATURE_REG *feature = reinterpret_cast<volatile CSCAN_ARS200FPGA_CH_FEATURE_REG *>(section.entry[1]);
    monitor.nGate1SyncTime = feature->wGate1SyncTime;
//	monitor.nGate1MPFirstTime = cscan_ch_feature_reg->wGate1MPFirstTime;
    monitor.nGate1MPTime = feature->wGate1MPTime;
    monitor.nGate1MPAmp = feature->wGate1MPAmp;
//	monitor.nGate1FPFirstTime = cscan_ch_feature_reg->wGate1FPFirstTime;
//	monitor.nGate1FPTime = cscan_ch_feature_reg->wGate1FPTime;
//	monitor.nGate1FPAmp = cscan_ch_feature_reg->wGate1FPAmp;
//	monitor.nGate2SyncTime = cscan_ch_feature_reg->wGate2SyncTime;
//	monitor.nGate2MPFirstTime = cscan_ch_feature_reg->wGate2MPFirstTime;
    monitor.nGate2MPTime = feature->wGate2MPTime;
    monitor.nGate2MPAmp = feature->wGate2MPAmp;
    monitor.nGate2FPFirstTime = feature->wGate2FPFirstTime;
    monitor.nGate2FPTime = feature->wGate2FPTime;
    monitor.nGate2FPAmp = feature->wGate2FPAmp;
    monitor.nGate3MPTime = feature->wGate3MPTime;
    monitor.nGate3MPAmp = feature->wGate3MPAmp;
    monitor.nGate3FPFirstTime = feature->wGate3FPFirstTime;
    monitor.nGate3FPTime = feature->wGate3FPTime;
    monitor.nGate3FPAmp = feature->wGate3FPAmp;
    monitor.nGate4MPTime = feature->wGate4MPTime;
    monitor.nGate4MPAmp = feature->wGate4MPAmp;
    monitor.nGate4FPFirstTime = feature->wGate4FPFirstTime;
    monitor.nGate4FPTime = feature->wGate4FPTime;
    monitor.nGate4FPAmp = feature->wGate4FPAmp;
}

bool Device::checkLevel(int channelIndex, int level)
{
    if (channelIndex >= channels.size()) {
        return false;
    }
    Channel &channel = channels[channelIndex];
    if (level >= channel.size()) {
        return false;
    }
    Section &section = channel[level];
    size_t bytes = section.sectionSize * section.sectionCount;
//    *section.reset = 0;

    if (*section.flag) {
        if (section.updateInterval > 0 && (section.elapsed.isValid() && !section.elapsed.hasExpired(section.updateInterval))) {
//            qCDebug(debug, "Skipping frame: %d, %d", section.elapsed.elapsed(), section.updateInterval);
            return false;
        }
        section.elapsed.start();

        if (section.update) {
            (this->*(section.update))(channelIndex);
        }
        int bufferIndex = *section.bufferIndex;
        int index = bufferIndex <= 2 ? bufferIndex - 1 : 0;
        quint32 type = 0x10000;

        FpgaBufferHeader header;
        header.channel = channelIndex;
        header.sectionSize = quint32(section.sectionSize);
        header.sectionCount = section.sectionCount;
        header.levelOfService = level;
        header.timeMultiplier = ioread<quint16>(0x060044 + channelIndex * 0x040000) + 1;
        const char *sections[] = {
            reinterpret_cast<const char *>(&type),
            reinterpret_cast<const char *>(&header),
            const_cast<const char *>(reinterpret_cast<volatile const char *>(section.entry[index]))
        };
        size_t sectionLen[] = {sizeof(type), sizeof(header), bytes};
        for (Client *client: Client::engines) {
            client->onDataReady(channelIndex, level);
//            qCDebug(debug) << "Data ready at channel" << channelIndex << "level" << level;
            if (client->dataLevel[channelIndex][level]) {
                if (client->socket->bytesToWrite() + bytes > maxSocketBuffer) {
                    qCDebug(debug, "Overrun: current %lld, adding %d", client->socket->bytesToWrite(), bytes);
                    client->overrun += bytes;
                    continue;
                }
                client->SendBinary(sections, sectionLen, 3);
            }
        }
        *section.reset = 1;
        //Sleep(1);
        *section.reset = 0;
        if (*section.flag) {
            qCDebug(debug, "FPGA failed resetting buffer state");
        }
        return true;
    }
    return false;
}
