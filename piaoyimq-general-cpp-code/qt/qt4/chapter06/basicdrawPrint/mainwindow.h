#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFontComboBox>
#include "form.h"
#include "palette.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void draw(QAction* action);
    void print();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
	void createDockWindows();

	Palette *paletteWidget;
    Form *form;

	QMenu *fileMenu;
    QMenu *drawMenu;
    QToolBar *fileToolBar;
    QToolBar *drawToolBar;
	QFontComboBox *fontCmb;

	QAction *lineAct;
	QAction *polylineAct;
	QAction *rectangleAct;
	QAction *polygonAct;
	QAction *arcAct;
	QAction *pieAct;
	QAction *chordAct;
    QAction *ellipseAct;
	QAction *textAct;
	QAction *printAct;
	QActionGroup *drawActGroup;
};

#endif
