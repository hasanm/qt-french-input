#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QtWidgets>
#include <string>


QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QVBoxLayout;
class QIcon;
class QLibrary;
class QMenu;
class QSystemTrayIcon;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
 public:
  MainWindow();
  ~MainWindow();
  void setWindowSizeLocation();
  void onQuit();

  void keyDown(DWORD key);
  void keyUp(DWORD key);

public slots:
  void iconActivated(QSystemTrayIcon::ActivationReason);


private:
  int totalItems = 3;
  QPushButton *quitButton;
  QVBoxLayout *contentLayout;

  HHOOK hhkLowLevelKybd;

  QSystemTrayIcon* trayIcon;
  QMenu* trayIconMenu;

  void inputOE();
  void SendInputString(const std::wstring &str);

  QMenu* createMenu();
};

#endif // MAINWINDOW_H
