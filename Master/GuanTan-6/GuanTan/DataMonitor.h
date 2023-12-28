#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QObject>

class DataMonitor : public QObject
{
	Q_OBJECT

public:
	DataMonitor(QObject *parent = 0);
	~DataMonitor();

public:
	void SendADataReceived();
	void SendCDataReceived();
	void SendAScanCDataReceived();

signals:
	void AScanDataChanged();
	void CScanDataChanged();
	void AScanCDataChanged();

private:
	
};

#endif // DATAMONITOR_H
