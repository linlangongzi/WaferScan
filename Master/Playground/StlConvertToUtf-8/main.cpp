#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QStringMatcher>


void StlTransfer(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite)) {
        return;
    }
    QProgressDialog progress;
    progress.setLabelText(QString("正在读取STL文件:%1").arg(path));
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimumDuration(0);

    QTextStream stream(&file);

    QFileInfo fileInfo(path);
    QString data;
    QTextStream streamOut(&data, QIODevice::WriteOnly);

    while (!stream.atEnd()) {
        static const QStringMatcher pattern("solid ");
        QString line = stream.readLine();
        if (pattern.indexIn(line) >= 0) {
            line = "solid " + fileInfo.completeBaseName();
        }
        streamOut << line << endl;
    }
    stream.seek(0);
    file.resize(0);
    stream.setCodec("UTF-8");
    stream << data;

    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QString dir = QFileDialog::getExistingDirectory(NULL, QString("选择原STL文件目录"), QString(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//    qDebug() << "Dir is " << dir;
    QStringList pathList = QFileDialog::getOpenFileNames(NULL, QString("导入所有STL文件"), QString(), "(*.STL *.stl)");
    foreach (auto p, pathList) {
        qDebug() << "File name: " << p;
        StlTransfer(p);
    }
//    a.closeAllWindows();
    return 0;
}
