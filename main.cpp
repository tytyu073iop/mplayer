#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QQuickStyle>
#include "src/appcontroller.h"

int main(int argc, char *argv[])
{
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    qputenv("QT_QUICK_CONTROLS_THEME", "Dark");
#endif

    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("mplayer"));
    app.setOrganizationName(QStringLiteral("mplayer"));

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    QQuickStyle::setStyle(QStringLiteral("Fusion"));
#endif

    QQmlApplicationEngine engine;

    AppController controller(&engine);

    engine.rootContext()->setContextProperty(QStringLiteral("appController"), &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule(QStringLiteral("mplayer"), QStringLiteral("Main"));

    return QGuiApplication::exec();
}
