#include <QtWidgets>
#include <QLabel>
#include <QVBoxLayout> 
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"

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

  // Install the low-level keyboard & mouse hooks
  hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
  
  QWidget *root = new QWidget(this);
  QWidget *top = new QWidget(this);



  /* Top Layout */ 
  QHBoxLayout *topLayout = new QHBoxLayout(top);
  quitButton = new QPushButton(QString ("Quit"), this);
  connect(quitButton, &QPushButton::clicked, this, &MainWindow::onQuit); 
  topLayout->addWidget(quitButton);

  muteButton = new QPushButton(QString("Toggle Mute"), this);
  muteButton->setStyleSheet("background-color:green");  
  connect(muteButton, &QPushButton::clicked, this, &MainWindow::inputOE);
  topLayout->addWidget(muteButton);

  top->setLayout(topLayout);

  /* Content Layout */
  
  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);


  rootLayout->addWidget(top);
  // rootLayout->addWidget(middle);
  setCentralWidget(root);

  // Transparency
  setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setParent(0); // Create TopLevel-Widget
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  // setAttribute(Qt::WA_PaintOnScreen);
}

void MainWindow::onQuit() {
  qApp->quit();
}

void MainWindow::inputOE() {
  
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
  } else if (key == VK_F23 ) {
    qDebug() << "F23 Pressed";
  } else if (key == VK_F22 ) {
      qDebug() << "F22 Pressed";
  }
}

void MainWindow::keyUp(DWORD key){
} 

