#ifndef QPID_H
#define QPID_H

/*
  QPID created by Chris Konstad (October 11, 2011).
  QPID is a Qt C++ class that abstracts the use of P, PI, PID, and PD
  algorithms.

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

  I took an idea (the constraining of the ouput range) from
  http://www.arduino.cc/playground/Code/PIDLibrary.  The rest of this
  library was written by me with Wikipedia as a reference.

*/

#include <QObject>
#include <QTime>

class QPID : public QObject
{
    Q_OBJECT
public:
    explicit QPID(QObject *parent = 0);

signals:
    void errorZero();   //no error

    void errorChanged();    //error has changed

    void errorPositive();   //error is a positive number

    void errorNegative();   //error is a negative number

    void errorExists(); //an error exists

public slots:
    void configP(double kp, double outMin, double outMax);  //P
    void configPI(double kp, double ki, double outMin, double outMax);  //PI
    void configPID(double kp, double ki, double kd, double outMin, double outMax);  //PID
    void configPD(double kp, double kd, double outMin, double outMax);  //PD

    void setDirection(bool direct); //Set the direction (direct or reverse)
    bool getDirection();    //Get the currently set direction

    void setSetPoint(double set);   //Set the setPoint
    double getSetPoint();   //Get the set setPoint

    double calculate(double in);    //calculate the P(I)(D) value for the input value

    int getType();  //Retreive formula type(0: P, 1: PI, 2: PID, 3: PD, 4: None set)

private slots:

private:
    QTime *timeChecker; //used to find deltaTime

    double kP;  //Proportional constant
    double kI;  //Integral constant
    double kD;  //Derivative constant

    double outputMin;   //Minimum output value
    double outputMax;   //Maximum output value

    double integral;    //Integral:  Amount of displacement that adds up over time
    double derivative;  //Derivative:  Current rate of change
    double error;   //Proportional:  How far off of the setPoint you are
    double previousError;   //Used to find the change of error
    double setPoint;    //Target value

    double deltaTime;  //How much time has elapsed

    bool direction;  //True: direct, False: reverse
    //int interval;   //Interval for readings

    bool pAlgorithm;    //Use only proportional (error)
    bool piAlgorithm;   //Use proportional-integral (error + amount of error*time, used to fix offsets)
    bool pidAlgorithm;  //Use proportional-integral-derivative (error + amount of error*time + rate of change)
    bool pdAlgorithm;   //Use proportional-derivative (error + rate of change)

};

#endif // QPID_H
