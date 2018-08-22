#include <QMessageBox>

#include "dialog.h"
#include "ui_dialog.h"
#include "mattoqimage.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

QList<QList<int>> ipList;
QList<int> ipListt;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog), count(0)
{
    ui->setupUi(this);

    ui->loopStopBtn->setDisabled(true);
    ui->num->setText(QString("%1").arg(count));

    server = new Server(this);

    server->listen(QHostAddress::Any, 1234);

    connect(ui->sendBtn, SIGNAL(clicked()), this, SLOT(sendMsg()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearMsg()));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showConnection()
{
    count++;

    /* add socket object that join in */
    ui->objectBox->addItem(QString("%1").arg(server->socketList.last()));

    /* change connect number while connection is connecting */
    ui->num->setText(QString("%1").arg(count));

}

void Dialog::showDisconnection(int socketDescriptor)
{
    count--;

    /* remove disconnect socketdescriptor from list */
    server->socketList.removeAll(socketDescriptor);
    for(int i=0;i<ipListt.length();i++)
    {
        QList<int> a;
        a.append(socketDescriptor);
        a.append(ipListt[i]);
        int t=ipList.removeAll(a);
        qDebug()<<ipListt;
        qDebug()<<ipList;
        qDebug()<<t;
        a.clear();
    }

    /* reload combobox */
    ui->objectBox->clear();

    for (int i = 0; i < server->socketList.size(); i++) {
        ui->objectBox->addItem(QString("%1").arg(server->socketList.at(i)));
    }

    //change connect number while connection is disconnecting
    ui->num->setText(QString("%1").arg(count));
}


void Dialog::sendMsg()
{
    /* if send message is null return */
    if (ui->sendMsg->text() == "") {
        QMessageBox::information(NULL,
                                 tr("注意"),
                                 tr("发送内容不能为空！"),
                                 QMessageBox::Yes);

        return ;
    }

    {
        /* send original data */
        emit sendData(ui->sendMsg->text().toLocal8Bit(), ui->objectBox->currentText().toInt());
    }

    ui->sendMsg->setText("");
}

void stringToHtmlFilter(QString &str)
{
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}

void stringToHtml(QString &str, QColor color)
{
    QByteArray data;

    if (str.isEmpty()) {
        return;
    }

    data.append(color.red());
    data.append(color.green());
    data.append(color.blue());
    QString strColor(data.toHex());

    str = QString("<span style=\" color:#%1;\">%2</span>").arg(strColor).arg(str);
}


void Dialog::revData(QString peerAddr, QByteArray data)
{
    static QByteArray jpeg_string;
    int i = 0;
    jpeg_string+=data;
    if(jpeg_string.endsWith("end"))
    {

        img.loadFromData(jpeg_string);
        ui->label_5->setPixmap(QPixmap::fromImage(img)); //将图片显示在QLabel上
        qDebug()<<jpeg_string.count();

        jpeg_string.clear();

    }

//    msg = QString::fromLocal8Bit(data);
//    peerAddr.insert(peerAddr.size(), ": ");
//    stringToHtmlFilter(peerAddr);
//    stringToHtml(peerAddr, QColor(255, 0, 0));

//    msg.prepend(peerAddr);

//    ui->msg->append(msg);
}

void Dialog::clearMsg()
{
    ui->msg->clear();
}


Mat src2;
void Dialog::on_pushButton_clicked()
{
    QString path="D:\\IMG_20161205_114535.jpg";


    Mat src1 = imread( string((const char *)path.toLocal8Bit()) );//读入一站图片 暂时读入桌面上的一个文件
    cvtColor(src1,src2,CV_BGR2RGB);//opencv读取图片按照BGR方式读取，为了正常显示，所以将BGR转为RGB
    QImage showImage((const uchar*)src2.data,src2.cols,src2.rows,src2.cols*src2.channels(),QImage::Format_RGB888);
    ui->label_5->setPixmap(QPixmap::fromImage(showImage)); //将图片显示在QLabel上

}



Mat Dialog::TransBufferToMat(unsigned char* pBuffer, int nWidth, int nHeight, int nBandNum, int nBPB)
{
    cv::Mat mDst;
    if (nBandNum == 4)
    {
        if (nBPB == 1)
        {
            mDst = cv::Mat::zeros(cv::Size(nWidth, nHeight), CV_8UC4);
        }
        else if (nBPB == 2)
        {
            mDst = cv::Mat::zeros(cv::Size(nWidth, nHeight), CV_16UC4);
        }
    }
    else if (nBandNum == 3)
    {
        if (nBPB == 1)
        {
            mDst = cv::Mat::zeros(cv::Size(nWidth, nHeight), CV_8UC3);
        }
        else if (nBPB == 2)
        {
            mDst = cv::Mat::zeros(cv::Size(nWidth, nHeight), CV_16UC3);
        }
    }
    else if (nBandNum == 1)
    {
        if (nBPB == 1)
        {
            mDst = cv::Mat::zeros(cv::Size(nWidth, nHeight), CV_8UC1);
        }
        else if (nBPB == 2)
        {
            mDst = cv::Mat::zeros(cv::Size(nWidth, nHeight), CV_16UC1);
        }
    }

    for (int j = 0; j < nHeight; ++j)
    {
        unsigned char* data = mDst.ptr<unsigned char>(j);
        unsigned char* pSubBuffer = pBuffer + (nHeight - 1 - j) * nWidth  * nBandNum * nBPB;
        memcpy(data, pSubBuffer, nWidth * nBandNum * nBPB);
    }
    if (nBandNum == 1)
    {
        cv::cvtColor(mDst, mDst, CV_GRAY2BGR);
    }
    else if (nBandNum == 3)
    {
        cv::cvtColor(mDst, mDst, CV_RGB2BGR);
    }
    else if (nBandNum == 4)
    {
        cv::cvtColor(mDst, mDst, CV_RGBA2BGR);
    }

    return mDst;
}


