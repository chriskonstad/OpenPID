#include "qin.h"
#include <QtDebug>
#include <iostream.h>

QIn::QIn(QPID *pidPointer, QObject *parent) :
    QObject(parent)
{
    if(pidPointer)
        pid = pidPointer;
    else
        qDebug() << "PID pointer is null";

    timer = new QTimer(this);
    timer->setInterval(20); //50Hz
    connect(timer, SIGNAL(timeout()), this, SLOT(checkForData()));
    timer->start();
}

void QIn::checkForData()
{
    //check CIN
    QFile in;
    in.open(stdin, QIODevice::ReadOnly);
    QString inputData = in.readLine();  //BLOCKING
    in.close();

    if(!inputData.isEmpty())    //if there is new data
    {
        std::cout << pid->calculate(inputData.toDouble()) << std::endl;
    }
}
