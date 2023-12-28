#pragma once

#include <QWizard>

namespace Ui {
class WizardDatum;
}

class Axis;

class WizardDatum : public QWizard
{
    Q_OBJECT

public:
    explicit WizardDatum(QWidget *parent = 0);
    ~WizardDatum();

    QList<Axis *> selectedAxis() const;
    bool validateCurrentPage() override;
    void done(int result) override;
    void setStatus(const QString &text);

private slots:
    void onPageChanged(int pageId);
    void onQueueStateUpdated(int state, int progress, int total);

private:
    Ui::WizardDatum *ui;
};
