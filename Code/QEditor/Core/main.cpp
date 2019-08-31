#include "QEditor.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>

int main(int argc, char *argv[])
{
    // The editor leaks memory. But who cares? (:-)
    // _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    QApplication a(argc, argv);

    QPixmap pixmap("editorLoader.png");

    QSplashScreen splash(pixmap);
    splash.setStyleSheet("color: #EEEEEE;");
    splash.show();

    a.processEvents();

    QEditor editor;

    if (!editor.InitializeEngine(splash))
        return false;

    if (!editor.InitializeEntities(splash))
        return false;

    if (!editor.InitializeEntityPanels(splash))
        return false;

    splash.close();

    editor.showMaximized();

    do
    {
        a.processEvents();

    } while (editor.Update());

    return true;
}
