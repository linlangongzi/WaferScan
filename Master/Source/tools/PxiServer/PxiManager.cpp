#include "PxiManager.h"
#include "p9054_lib.h"
#include <QLoggingCategory>
#include "PxiCard.h"

static Q_LOGGING_CATEGORY(debug, "pxi")

static const quint32 vendorId = 0x10b5;
static const quint32 deviceId = 0x9054;
static const DWORD MULTI_CARD_FLAG = 0x0101;
static const DWORD AEC_CARD_FLAG = 0x0201;

PxiManager::PxiManager(QObject *parent) :
    QObject(parent)
{
}

PxiManager::~PxiManager()
{
}

void PxiManager::rescan()
{
    qDeleteAll(cards);
    cards.clear();

    qCDebug(debug, "Rescanning PXI bus...");

    int cardCount = P9054_CountCards(vendorId, deviceId);

    qCInfo(debug, "Found %d cards.", cardCount);

    for (int index = 0; index < cardCount; index++) {                            //查找卡
        P9054_HANDLE handle = NULL;
        P9054_Open(&handle, vendorId, deviceId, index, 0);

        if (handle == NULL) {
            qCCritical(debug) << "Failed opening device" << index;
            continue;
        }

        PxiCard *card = new PxiCard(handle, this);
        card->index = index;
        cards << card;

        card->dumpInfo();
    }

    qCDebug(debug) << "Sorting cards...";
    std::sort(cards.begin(), cards.end(), [] (const PxiCard *card1, const PxiCard *card2) {
        const auto &s1 = card1->handle->pciSlot;
        const auto &s2 = card2->handle->pciSlot;
        return (s1.dwBus > s2.dwBus) || ((s1.dwBus == s2.dwBus) && (s1.dwSlot < s2.dwSlot));
    });
    qCInfo(debug) << "Sorted devices:";
    for (const auto *card: cards) {
        card->dumpInfo();
    }
    qCInfo(debug) << "Performing RAM test...";
    for (auto *card: cards) {
        card->ramTest(0xC00, 1024);
    }
}
