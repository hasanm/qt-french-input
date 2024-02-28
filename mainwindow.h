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


private:
  int totalItems = 3;
  QPushButton *quitButton;
  QPushButton *muteButton;
  QVBoxLayout *contentLayout;

  HHOOK hhkLowLevelKybd;

  void inputOE();
  void SendInputString(const std::wstring &str);

};

#endif // MAINWINDOW_H
