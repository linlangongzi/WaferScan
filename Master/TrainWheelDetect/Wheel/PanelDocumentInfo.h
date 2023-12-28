#ifndef PANELDOCUMENTINFO_H
#define PANELDOCUMENTINFO_H

#include <QFrame>
#include <Core/DocumentInfo.h>

namespace Ui {
class PanelDocumentInfo;
}

class PanelDocumentInfo : public QFrame
{
    Q_OBJECT

public:
    explicit PanelDocumentInfo(QWidget *parent = 0);
    ~PanelDocumentInfo();

public slots:
    void refresh();
    void onDocumentChanged();
    void actionModify(bool checked);

private:
    Ui::PanelDocumentInfo *ui;
};

#endif // PANELDOCUMENTINFO_H
