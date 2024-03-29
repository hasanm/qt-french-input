#include <QtWidgets>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"
#include "Windows.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

// Crosslink between Qt class and win callback
MainWindow *mwReference;


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode == HC_ACTION) {
    switch (wParam) {
      // Pass KeyDown/KeyUp messages for Qt class to logicize
    case WM_KEYDOWN:
      mwReference->keyDown(PKBDLLHOOKSTRUCT(lParam)->vkCode);
      break;
    case WM_KEYUP:
      mwReference->keyUp(PKBDLLHOOKSTRUCT(lParam)->vkCode);
      break;
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

MainWindow::~MainWindow(){
  // Be friendly! Remove hooks!
  UnhookWindowsHookEx(hhkLowLevelKybd);
}

MainWindow::MainWindow()
{
  mwReference = this;

  auto menu = this->createMenu();
  trayIcon = new QSystemTrayIcon(this);
  this->trayIcon->setContextMenu(menu);

  auto appIcon = QIcon(":/icons/heart.png");
  this->trayIcon->setIcon(appIcon);
  this->setWindowIcon(appIcon);

  this->trayIcon->show();

  connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

  // Install the low-level keyboard & mouse hooks
  hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

  QWidget *root = new QWidget(this);
  QWidget *top = new QWidget(this);

  /* Top Layout */
  QHBoxLayout *topLayout = new QHBoxLayout(top);

  quitButton = new QPushButton(QString ("Quit"), this);
  connect(quitButton, &QPushButton::clicked, this, &MainWindow::onQuit);
  topLayout->addWidget(quitButton);

  top->setLayout(topLayout);

  /* Content Layout */

  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);


  rootLayout->addWidget(top);
  setCentralWidget(root);
}

void MainWindow::onQuit() {
  qApp->quit();
}

void MainWindow::SendInputString(const std::wstring &str)
{
    int len = str.length();
    if (len == 0) return;

    std::vector<INPUT> in(len*2);
    ZeroMemory(&in[0], in.size()*sizeof(INPUT));

    int i = 0, idx = 0;
    while (i < len)
    {
        WORD ch = (WORD) str[i++];

        if ((ch < 0xD800) || (ch > 0xDFFF))
        {
            in[idx].type = INPUT_KEYBOARD;
            in[idx].ki.wScan = ch;
            in[idx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++idx;

            in[idx] = in[idx-1];
            in[idx].ki.dwFlags |= KEYEVENTF_KEYUP;
            ++idx;
        }
        else
        {
            in[idx].type = INPUT_KEYBOARD;
            in[idx].ki.wScan = ch;
            in[idx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++idx;

            in[idx].type = INPUT_KEYBOARD;
            in[idx].ki.wScan = (WORD) str[i++];
            in[idx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++idx;

            in[idx] = in[idx-2];
            in[idx].ki.dwFlags |= KEYEVENTF_KEYUP;
            ++idx;

            in[idx] = in[idx-2];
            in[idx].ki.dwFlags |= KEYEVENTF_KEYUP;
            ++idx;
        }
    }

    SendInput(in.size(), &in[0], sizeof(INPUT));

}

void MainWindow::inputOE() {
  QString test = "Hello";
  INPUT ip;
  ip.type = INPUT_KEYBOARD;
  ip.ki.time = 0;
  ip.ki.dwExtraInfo = 0;

  // Press the key
  // byte x = VkKeyScan(test.at(0).unicode());
  ip.ki.wVk = 0; // virtual-key code for the "a" key
  ip.ki.dwFlags = KEYEVENTF_UNICODE; // 0 for key press
  ip.ki.wScan = 0x0153;
  SendInput(1, &ip, sizeof(INPUT));

  // Release the key
  ip.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &ip, sizeof(INPUT));
}


void MainWindow::setWindowSizeLocation() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rec = screen->availableGeometry();
    qDebug() << "Hello " << rec.width() << " x " << rec.height();

    // int targetWidth = this->width();
    int targetWidth = 300;

    int height = 180;
    int width = rec.width();
    int x=(width - targetWidth);
    int y= 50;
    this->setGeometry(x,y,targetWidth,height);
}

void MainWindow::keyDown(DWORD key)
{
  if (key == VK_F24) {
    qDebug() << "F24 Presed";
    inputOE();
    // std::wstring st = L"œ";
    // SendInputString(st);
  }
}

void MainWindow::keyUp(DWORD key){
}


QMenu* MainWindow::createMenu() {

  auto quitAction = new QAction("&Quit", this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

  auto menu = new QMenu(this);
  menu->addAction(quitAction);
  return menu;

}



void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_) {
  auto icon = QIcon(":/icons/heart.png");

  switch (reason_) {
  case QSystemTrayIcon::Trigger:
    this->trayIcon->showMessage(QString("Hello") , QString("No ") , icon, 5000);
    break;
  default:
    break;
  }
}