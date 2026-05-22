#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>

class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() {}

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        engine->addImportPath(TEST_SOURCE_DIR "/../");
    }
};

QUICK_TEST_MAIN_WITH_SETUP(tst_qml_controls, Setup)
#include "tst_qml_controls.moc"
