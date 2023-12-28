#include "SafetyBarrierOptions.h"

REGISTER_JSON_CONVERTER(SafetyBarrierOptions)

SafetyBarrierOptions SafetyBarrierOptions::fromJson(const QJsonValue &json)
{
    const QJsonObject obj = json.toObject();
    SafetyBarrierOptions options;
    if (obj.contains("Rect")) {
        options.rect = convertFromJson<QRectF>(obj["Rect"]);
        options.minDepth = obj["EmptyHeight"].toDouble();
        options.maxDepth = obj["MaxDepth"].toDouble();
        return options;
    }
    JsonToGadget(json, SafetyBarrierOptions::staticMetaObject, &options);
    return options;
}
