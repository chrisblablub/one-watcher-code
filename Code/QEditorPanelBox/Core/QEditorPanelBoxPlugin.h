/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifndef QEDITORPANELBOXPLUGIN_H
#define QEDITORPANELBOXPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QtUiPlugin/customwidget.h>
#include <QtDesigner/QDesignerContainerExtension>
#include "DLLExportImport.h"

class DLL_EXPORT QEditorPanelBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "qeditorpanelboxplugin.json")
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	QEditorPanelBoxPlugin(QObject *parent = 0);

	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget *createWidget(QWidget *parent);
	void initialize(QDesignerFormEditorInterface *core);

private:
	bool initialized;
};

#endif // QEDITORPANELBOXPLUGIN_H
