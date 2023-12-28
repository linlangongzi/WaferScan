#include "DataMonitor.h"

DataMonitor::DataMonitor(QObject *parent)
	: QObject(parent)
{

}

DataMonitor::~DataMonitor()
{

}

void DataMonitor::SendADataReceived()
{
	emit AScanDataChanged();
}

void DataMonitor::SendCDataReceived()
{
	emit CScanDataChanged();
}

void DataMonitor::SendAScanCDataReceived()
{
	emit AScanCDataChanged();
}