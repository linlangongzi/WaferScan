#include "NBT_47013_2015.h"
IMPLEMENT_RUNTIME(NBT_47013_2015)

NBT_47013_2015::NBT_47013_2015(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

NBT_47013_2015::~NBT_47013_2015()
{

}

void NBT_47013_2015::SetPartProperty()
{
	// All
	mapPartProperty.insert("DefectMaxs", false);
	mapPartProperty.insert("MaxAreaAll", false);
	mapPartProperty.insert("MaxLengthAll", false);
	mapPartProperty.insert("MaxIndiLengthAll", false);

	// Body
	mapPartProperty.insert("MaxAreaBody", false);
	mapPartProperty.insert("PerSquareMeterMaxNumBody", false);

	// Edge
	mapPartProperty.insert("MaxAreaEdge", false);
	mapPartProperty.insert("MaxLengthEdge", false);
	mapPartProperty.insert("MaxIndiLengthEdge", false);
	mapPartProperty.insert("PerMeterMaxNumEdge", false);
    
	// Other
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);
}

void NBT_47013_2015::SetDefectRank(PUTSRecognizer::Defect &defect)
{
	// 默认B-I等级
	double maxAreaBody = options.value("MaxAllowedAreaBody").toDouble(48);

	// 默认E-I等级
	double maxAreaEdge = options.value("MaxAllowedAreaEdge").toDouble(48);
	double maxLengthEdge = options.value("MaxAllowedLengthEdge").toDouble(20);

	if (defect.type == DEFECT_NORMAL)
	{
		defect.rank = defect.area / maxAreaBody;
	}
	else if (defect.type == DEFECT_EDGE)
	{
		defect.rank = qMax(defect.length / maxLengthEdge, defect.area / maxAreaEdge);
	}
	else if (defect.type == DEFECT_COMBINED_BODY)
	{
		defect.rank = defect.area / maxAreaBody;
	}
	else if (defect.type == DEFECT_COMBINED_EDGE)
	{
		defect.rank = qMax(defect.length / maxLengthEdge, defect.area / maxAreaEdge);
	}
	else
	{
		;//
	}
}