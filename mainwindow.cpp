#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("UartAssisant");
    setWindowIcon(QIcon(":/images/Images/logo.ico"));

    pAboutui = new AboutDialog();

    Serialstatus = new QLabel(this);
    Main_SerialPort = new QSerialPort(this);
    Serial_Ports = new QList<QSerialPort*>();

    Serialstatus->setText(this->String_SerialPort_status);


    this->flush_serial_list();
    QSerialPort com(ui->cb_seriallist->currentText(),this);
    this->setMainSerialPort(com.portName());


    ui->BT_send->setDisabled(true);
    //Serialstatus->setText(this->get_SerialPort_Status_as_String());
    //statusBar()->addWidget(Serialstatus);// 状态栏唯一，直接调用，不用定义对象
    //  bug list
    //this->String_SerialPort_status = this->get_SerialPort_Status_as_String();



    //      信号发出者         对象发出的信号     信号接受者       槽函数
    connect(ui->action_quit ,SIGNAL(triggered()),this,SLOT(MainWindow_Quit()));
    connect(ui->actions_about,SIGNAL(triggered()),this,SLOT(MainWindow_About()));
    connect(ui->action_flush,SIGNAL(triggered()),this,SLOT(flush_serial_list()));
    connect(this->Main_SerialPort, SIGNAL(readyRead()), this, SLOT(SerialReadData()));










    ui->sendtext->setFocus();
    qDebug() << this->Main_SerialPort->portName() << endl;
}

MainWindow::~MainWindow()
{
    delete ui;
//    delete Serialstatus;

}
bool MainWindow::setMainSerialPort(QString PortName)
{
    QSerialPortInfo comif ;
    QSerialPort* com = new QSerialPort(PortName,this);
    com->setBaudRate(QSerialPort::Baud9600);
    com->setDataBits(QSerialPort::Data8 );
    com->setStopBits(QSerialPort::OneStop );
    com->setParity(QSerialPort::NoParity );
    comif = QSerialPortInfo(*com);


    if (comif.isBusy())
    {
        qDebug() << PortName << "is busy!!" << endl;
        return false;
    }
    else
    {
        Main_SerialPort = com;
        qDebug() << "Main Port is" << Main_SerialPort->portName() << endl;
        return true;
    }


    // Main_SerialPort->close();
    //Main_SerialPort->clear(QSerialPort::AllDirections) ;

}
QString MainWindow::get_SerialPort_Status_as_String()
{
    QString  status_string;
    status_string.append( this->Main_SerialPort->portName());
    status_string.append( "-");
    status_string.append( QString::number( this->Main_SerialPort->baudRate() ));
    status_string.append( "-");
    status_string.append( QString::number(this->Main_SerialPort->dataBits()));
    status_string.append( "-");
    // 校验
    switch (this->Main_SerialPort->parity()) {
    case QSerialPort::NoParity :
        status_string.append( "N");
        break;
    case QSerialPort::EvenParity:
        status_string.append( "E");
        break;
    case QSerialPort::OddParity:
        status_string.append( "O");
        break;
    case QSerialPort::SpaceParity :
        status_string.append( "S");
        break;
    case QSerialPort::MarkParity :
        status_string.append( "M");
        break;
    case QSerialPort::UnknownParity :
        status_string.append( "U");
        break;

    default:
        break;
    }
    status_string .append( "-");
    // 停止位数
    switch (this->Main_SerialPort->stopBits()) {
    case QSerialPort::OneStop:
        status_string.append( "1");
        break;
    case QSerialPort::OneAndHalfStop:
        status_string.append( "1.5");
        break;
    case QSerialPort::TwoStop:
        status_string.append( "2");
        break;
    default:
        status_string.append( "U");
        break;
    }
    this->String_SerialPort_status = status_string;
    return status_string;

}

void MainWindow::SerialReadData()
{
    static QByteArray ReceiveDataBuffer;
    static std::string ReceiveString;
    ReceiveDataBuffer = Main_SerialPort->readAll();
    ReceiveString =  ReceiveDataBuffer.toStdString();
    ui->display->append("@"+QString::fromStdString(ReceiveString));

}

void MainWindow::MainWindow_About()
{
    qDebug() << "click about>aboutsoft " << endl ;
    if (pAboutui != NULL)
    {
        pAboutui->exec();
    }
}


void MainWindow::MainWindow_Quit()
{
    qDebug() << "click menu>quit " << endl;
    this->close() ;
}


void MainWindow::on_BT_openclosesrial_clicked()
{
    static bool MainSerialIsOpen = false;

    qDebug() << "[Open/Close port]" << this->Main_SerialPort->portName();


    //return;


    if (MainSerialIsOpen)
    {

        Main_SerialPort->close();
        MainSerialIsOpen = false;
        ui->BT_openclosesrial->setText("打开串口");
        ui->BT_send->setDisabled(true);


        qDebug() << "Close port: " << Main_SerialPort->portName()   <<endl;
    }
    else if (!MainSerialIsOpen)
    {
        if (Main_SerialPort!= NULL)
        {
            Main_SerialPort->open(QSerialPort::ReadWrite );
            MainSerialIsOpen = true;
            ui->BT_openclosesrial->setText("关闭串口");
            ui->BT_send->setEnabled(true);
        }
        else
        {
            qDebug() << "Serialport   NULL!" << endl;
        }




        qDebug() << "Open port: " << Main_SerialPort->portName()   <<endl;


    }
}

void MainWindow::on_BT_clearscreem_clicked()
{
    ui->display->clear();
}




void MainWindow::on_btn_flush_clicked()
{
    this->flush_serial_list();
}


void MainWindow::flush_serial_list()
{

    QSerialPort * p_serial = new QSerialPort();

    ui->cb_seriallist->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        p_serial->setPort(info);
        if (!info.isBusy())
        {
            // todo: 判断是否已存在info对应端口
            ui->cb_seriallist->addItem(p_serial->portName());//+info.description()
        }
        if (-1 == this->Serial_Ports->indexOf(p_serial))
            this->Serial_Ports->append(p_serial);

/*
        ui->display->insertPlainText( "Name : " + info.portName()+'\n');
        ui->display->insertPlainText( "Description : " + info.description()+'\n');
        ui->display->insertPlainText( "Manufacturer: " + info.manufacturer()+'\n');
        ui->display->insertPlainText( "Product Identifier: " + QString::number( info.productIdentifier())+'\n');
        ui->display->insertPlainText( "Serial Number: " + info.serialNumber()+'\n');
        ui->display->insertPlainText( "System Location: " + info.systemLocation()+'\n');
        ui->display->insertPlainText( "=====================================\n" );
*/


       qDebug() << "Name : "                << info.portName()               <<'\n';
/*       qDebug() << "Description : "         << info.description()            <<'\n';
       qDebug() << "Manufacturer: "         << info.manufacturer()           <<'\n';
       qDebug() << "Product Identifier: "   << QString::number( info.productIdentifier()  )    <<'\n';
       qDebug() << "Serial Number: "        << info.serialNumber()           <<'\n';
       qDebug() << "System Location: "      << info.systemLocation()         <<'\n';*/
       qDebug() << "=====================================\n" ;
    }







}

void MainWindow::on_cb_seriallist_activated(const QString &arg1)
{
    qDebug() << "actived" << arg1 <<endl;
    this->setMainSerialPort(arg1);
    //Serialstatus->setText(this->get_SerialPort_Status_as_String());
}


/*
void MainWindow::on_cb_seriallist_currentIndexChanged(int index)
{
    qDebug() << "currentIndexChanged" << index << endl;

}
*/


void MainWindow::on_BT_send_clicked()
{
    //const  char * data = "zhaojiajun\r\n";
    this->Serial_Send();

//    ui->BT_send->setDisabled(true);

}


void MainWindow::Serial_Send()
{
    QByteArray SendDataBuffer ;
    SendDataBuffer = ui->sendtext->toPlainText().toLatin1();
    if(!SendDataBuffer.isEmpty())
    {
        Main_SerialPort->write(SendDataBuffer);
        qDebug() << "[Send]["<< SendDataBuffer.length() <<"]" << SendDataBuffer << endl;
    }
    qDebug() << "click send botton"  << endl;
}

void MainWindow::on_BT_cleasenddata_clicked()
{
    ui->sendtext->clear();
}



void MainWindow::on_action_flush_triggered()
{
    this->flush_serial_list();
}

