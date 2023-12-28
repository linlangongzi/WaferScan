TEMPLATE = aux

CONFIG += file_copies

profiles.files = \
    AxisProfiles \
    ReportTemplates

profiles.base = $$PWD
profiles.path = $$DS_RESOURCE_PATH

COPIES = profiles
