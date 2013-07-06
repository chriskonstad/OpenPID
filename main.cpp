/*
 * Written by Chris Konstad (July 6, 2013)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * This application is a console application that allows for easy use of the
 * PID (proportional, integral, derivative) algorithm.  You start the application
 * with 6 arguments, documented below, that set up the algorithm.  Then, when
 * you send data to the application through stdin, it runs it through the PID
 * algorithm and sends the output through stdout. kP, kI, and kD are usually
 * REALLY SMALL VALUES (0.X, 0.0000XX and 0.0000XX, respectively).
 *
 *
 */

#include <QCoreApplication>
#include <QStringList>
#include "qpid.h"
#include "qin.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationVersion("1.0");

    QStringList arguments = QCoreApplication::arguments();

    //qDebug() << "starting application";
    QPID pid;
    //ARGUMENTS: kP, kI,kD, outMin, outMax, setPoint, direction (if 1, then make it indirect, else direct)
    //Example arguments to output a value between 0 and 100 while trying to move the input value to 50: 0.5 0.00015 0.0003 0 100 50 1
    pid.configPID(arguments.at(1).toDouble(), arguments.at(2).toDouble(), arguments.at(3).toDouble(), arguments.at(4).toDouble(), arguments.at(5).toDouble());
    pid.setSetPoint(arguments.at(6).toDouble());
    if(arguments.count() == 8)  //if the argument is passed
    {
        if(arguments.at(7).toInt() == 1)
            pid.setDirection(false);    //make indirect
    }

    QIn qin(&pid);  //object that waits for the new data to come along

    return a.exec();
}
