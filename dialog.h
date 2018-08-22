#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include "mattoqimage.h"
#include <opencv2/opencv.hpp>
#include "server.h"

namespace Ui {
class Dialog;
}

class Server;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void sendData(QByteArray data, int id);

public slots:
    void revData(QString peerHost, QByteArray data);

private slots:
    void showConnection();
    void showDisconnection(int socketDescriptor);
    void sendMsg();
    void clearMsg();

    void on_pushButton_clicked();
    Mat TransBufferToMat(unsigned char* pBuffer, int nWidth, int nHeight, int nBandNum, int nBPB );
private:
    Ui::Dialog *ui;

    int count;

    Server *server;
    Mat picture;
    QImage img;
};




#endif // DIALOG_H
