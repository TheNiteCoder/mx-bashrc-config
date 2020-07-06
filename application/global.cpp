#include "global.h"

ExecuteResult runCmd(QString cmd, bool interactive, bool onlyStdout)
{
    SCOPE_TRACKER;
    QEventLoop loop;
    QProcess proc;
    if(!onlyStdout)
        proc.setReadChannelMode(QProcess::MergedChannels);
    QObject::connect(&proc, QOverload<int>::of(&QProcess::finished), &loop, &QEventLoop::quit);
    DEBUG << "Execute Statment: command: " << "/bin/bash";
    DEBUG << "Execute Statment: args: " << (QStringList() << ((interactive) ? "-ic" : "-c") << cmd);
    proc.start("/bin/bash", (QStringList() << ((interactive) ? QString("-ic") : QString("-c")) << cmd));
    loop.exec();
    QObject::disconnect(&proc, nullptr, nullptr, nullptr);
    ExecuteResult result;
    result.rv = proc.exitCode();
    result.output = proc.readAll(); //remove .trimmed() because all spaces count in output
    proc.close();
    return result;
};

