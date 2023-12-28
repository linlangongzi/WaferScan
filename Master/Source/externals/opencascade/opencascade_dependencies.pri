DS_LIB_NAME = opencascade

OCCT_ROOT = $$clean_path($$DS_OPEN_CASCADE_ROOT/opencascade-7.1.0)
OCCT_MODULES = \
    TKernel \
    TKMath \
    TKService \
    TKV3d \
    TKOpenGl \
    TKBRep \
    TKIGES \
    TKSTL \
    TKVRML \
    TKSTEP \
    TKSTEPAttr \
    TKSTEP209 \
    TKSTEPBase \
    TKGeomBase \
    TKGeomAlgo \
    TKG3d \
    TKG2d \
    TKXSBase \
    TKShHealing \
    TKHLR \
    TKTopAlgo \
    TKMesh \
    TKPrim \
    TKCDF \
    TKBool \
    TKBO \
    TKFillet \
    TKOffset \

OCCT_PLATFORM = $$OCCT_ROOT/win64/vc10

INCLUDEPATH += $$OCCT_ROOT/inc
LIBS += -L$$OCCT_PLATFORM/lib

for(module, OCCT_MODULES) {
    LIBS += -l$${module}
}
