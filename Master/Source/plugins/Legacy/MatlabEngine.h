#pragma once

#include <QLibrary>

class MatlabEngine : public QObject
{
    Q_OBJECT

public:
    MatlabEngine(QObject *parent = nullptr);
    ~MatlabEngine() override;
    void SetBasePath(const QString &path) {basePath = path;}
    bool Load();
    bool IsValid() const {return engine;}
    QString DoString(const QString &command);

private:
    using Engine = struct engine;
    using EngOpenFunc = Engine *(*)(const char *);
    using EngCloseFunc = int (*)(Engine *);
    using EngSetVisibleFunc = int (*)(Engine *, bool);
    using EngOutputBufferFunc = int (*)(Engine *, char *, int);
    using EngEvalStringFunc = int (*)(Engine *, const char *);

    QString basePath;
    QLibrary eng;
//    QLibrary mx;
    EngEvalStringFunc engEvalString = nullptr;
    Engine *engine = nullptr;
    QByteArray buffer;
};
