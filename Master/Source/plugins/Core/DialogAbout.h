#pragma once

#include <QDialog>
#include <QLabel>
#include <QPointer>
#include "CoreGlobal.h"

namespace Ui {
class DialogAbout;
}

/*!
 * DialogAbout 是一个“关于”对话框，负责显示App的基本信息和版本，显示和管理授权，在有效授权消失时
 * 挡住主窗口。
 *
 * 为了让DialogAbout正常显示App的基本信息，需要设置下列值：
 *  qApp->applicationDisplayName()  - App名称
 *  qApp->applicationVersion())     - App版本号
 *  qApp->property("Revision")      - App修订号，QString
 *  qApp->property("CopyrightTr")   - App的“版权所有”信息，翻译为当前语言，QString
 *  qApp->property("MakeSpec")      - Qt套件的名称，QString
 *  qApp->property("BuildDate")     - 构建日期，QDate
 *  qApp->property("BuildTime")     - 构建时间，QTime
 *  qApp->property("WebPortal")     - 公司网址，QString
 *  qApp->property("Telephone")     - 客服电话，QString
 *
 * 当前选定的授权设备，会显示在对话框中。
 * 若当前没有选定授权设备，或选定的授权设备消失，则对话框会弹出，并挡住其parent，配合
 * Qt::ApplicationModal，达到阻止用户使用主窗口的效果。
 * 授权设备相关的说明详见 LicenseManager 。
 *
 * 由于核心和主窗口加载可能需要选定一个授权设备，DialogAbout应当在main函数中构造，先于核心和主窗
 * 口加载。
 * 若没有任何授权设备可用，则使用exec()等待用户插入授权设备或退出。
 *
 * exec()返回时，若授权设备不存在，则用户选择了退出；
 * 若授权设备存在，在主窗口加载完毕后，应当将
 * DialogAbout的parent设置为主窗口，否则DialogAbout将无法正确挡住主窗口。
 *
 * \sa LicenseManager
 */
class CORE_EXPORT DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = 0);
    ~DialogAbout();

    virtual int exec() override;
    virtual void done(int r) override;
    static void addModuleCredit(const QString &name, const QString &path);

protected:
    virtual void showEvent(QShowEvent *e) override;

private slots:
    void refreshLicense();
    void on_credits_clicked();
    void on_editLicense_clicked();
    void on_credential_clicked();

private:
    Ui::DialogAbout *ui;
    bool isAutoOpened = false;                                                  /*!< 对话框是否由用户手动打开 */
    QPointer<QLabel> statusLabel;                                               /*!< 状态条标签, 用于提示授权即将过期. 由于状态条控件的parent为statusBar, 使用QPointer保证指针安全. */
};
