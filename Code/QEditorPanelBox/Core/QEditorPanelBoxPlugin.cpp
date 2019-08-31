#include "QEditorPanelBox.h"

#include <QtCore/QtPlugin>
#include "QEditorPanelBoxPlugin.h"


QEditorPanelBoxPlugin::QEditorPanelBoxPlugin(QObject *parent)
	: QObject(parent)
{
	initialized = false;
}

void QEditorPanelBoxPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
	if (initialized)
		return;

	initialized = true;
}

bool QEditorPanelBoxPlugin::isInitialized() const
{
	return initialized;
}

QWidget *QEditorPanelBoxPlugin::createWidget(QWidget *parent)
{
	return new QEditorPanelBox(parent);
}

QString QEditorPanelBoxPlugin::name() const
{
	return "QEditorPanelBox";
}

QString QEditorPanelBoxPlugin::group() const
{
	return "QEditor";
}

QIcon QEditorPanelBoxPlugin::icon() const
{
	return QIcon();
}

QString QEditorPanelBoxPlugin::toolTip() const
{
	return QString();
}

QString QEditorPanelBoxPlugin::whatsThis() const
{
	return QString();
}

bool QEditorPanelBoxPlugin::isContainer() const
{
	return true;
}

QString QEditorPanelBoxPlugin::domXml() const
{
	return "<widget class=\"QEditorPanelBox\" name=\"qEditorPanelBox\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString QEditorPanelBoxPlugin::includeFile() const
{
	return "QEditorPanelBox.h";
}

