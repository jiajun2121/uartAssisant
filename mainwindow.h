#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QDebug"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QStatusBar>
#include <QLabel>
#include <QByteArray>
#include <QByteArrayData>

#include <aboutdialog.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui = NULL;
    AboutDialog *pAboutui = NULL;

    QList<QSerialPort *> * Serial_Ports = NULL;
    QLabel * Serialstatus = NULL;
    QStatusBar * statusbar = NULL;
    QString * Default_Serial_Name = NULL;
    QSerialPort * Main_SerialPort = NULL;           //Serial are used for Send & receive data
    QString String_SerialPort_status = "";













    void Serial_Send();
    QString get_SerialPort_Status_as_String();
    bool setMainSerialPort(QString);


private slots:
    void MainWindow_Quit();
    void MainWindow_About();
    void flush_serial_list();
    void SerialReadData();


    void on_BT_openclosesrial_clicked();
    void on_BT_clearscreem_clicked();
    void on_btn_flush_clicked();

    void on_cb_seriallist_activated(const QString &arg1);
    void on_BT_send_clicked();
    void on_BT_cleasenddata_clicked();
    void on_action_flush_triggered();
//    void on_checkBox_hexsend_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
