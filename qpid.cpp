#include "qpid.h"

QPID::QPID(QObject *parent) :
    QObject(parent)
{
    timeChecker = new QTime;    //create QTime to find deltaTime

    //Set to default values
    pAlgorithm = false;
    piAlgorithm = false;
    pidAlgorithm = false;
    pdAlgorithm = false;
    kP = 0;
    kI = 0;
    kD = 0;
    outputMin = 0;
    outputMax = 100;
    integral = 0;
    derivative = 0;
    deltaTime = 0;
    direction = true;   //true = direct; false = indirect
    previousError = 0;  //set previous error to a stable value
    error = 0;

    setSetPoint(0); //set to default (zero) value

    timeChecker->start();   //start the timer
}

void QPID::configP(double kp, double outMin, double outMax)
{
    pAlgorithm = true;
    piAlgorithm = false;
    pidAlgorithm = false;
    pdAlgorithm = false;

    kP = kp;

    //Invert?
    if(!direction)   //if reverse,  then invert
    {
        kP = -kP;
    }

    outputMin = outMin;
    outputMax = outMax;

    integral = 0;

}

void QPID::configPI(double kp, double ki, double outMin, double outMax)
{
    pAlgorithm = false;
    piAlgorithm = true;
    pidAlgorithm = false;
    pdAlgorithm = false;

    kP = kp;
    kI = ki;

    //Invert?
    if(!direction)   //if reverse, then invert
    {
        kP = -kP;
        kI = -kI;
    }

    outputMin = outMin;
    outputMax = outMax;

    integral = 0;
}

void QPID::configPID(double kp, double ki, double kd, double outMin, double outMax)
{
    pAlgorithm = false;
    piAlgorithm = false;
    pidAlgorithm = true;
    pdAlgorithm = false;

    kP = kp;
    kI = ki;
    kD = kd;

    //Invert?
    if(!direction)   //if reverse, then invert
    {
        kP = -kP;
        kI = -kI;
        kD = -kD;
    }

    outputMin = outMin;
    outputMax = outMax;

    integral = 0;
}

void QPID::configPD(double kp, double kd, double outMin, double outMax)
{
    pAlgorithm = false;
    piAlgorithm = false;
    pidAlgorithm = false;
    pdAlgorithm = true;

    kP = kp;
    kD = kd;

    //Invert?
    if(!direction)   //if reverse, then swap
    {
        kP = -kP;
        kD = -kD;
    }

    outputMin = outMin;
    outputMax = outMax;

    integral = 0;
}

void QPID::setDirection(bool direct)
{
    direction = direct;
}

double QPID::calculate(double in)
{
    double _input = in;
    double result;

    deltaTime = timeChecker->restart(); //restart and get change of time
    //deltaTime = deltaTime / 1000;   //convert to seconds

    error = setPoint - _input;  //find error

    //emit signals based on error value
    if(error > 0)
    {
        emit errorPositive();
    }
    else if(error < 0)
    {
        emit errorNegative();
    }

    integral = integral + (error*deltaTime);    //find integral

    if(error == 0)  //reset integral value so that it doesn't overshoot too much
    {
        integral = 0;
        emit errorZero();   //emit signal if there is no error
    }

    derivative = (error - previousError)/deltaTime; //find derivative

    if(error != previousError)  //if the error has changed
    {
        emit errorChanged();
    }

    previousError = error;  //set the previous error to current error

    if(pAlgorithm)  //if only Proportional
    {
        result = (kP*error);

        //Make output value within output range
        if(result > outputMax)
        {
            result = outputMax;
        }
        if(result < outputMin)
        {
          result = outputMin;
        }

        return result;
    }

    if(piAlgorithm) //if Proportional-Integral
    {
        result = (kP*error) + (kI*integral);

        //Make output value within output range
        if(result > outputMax)
        {
            result = outputMax;
        }
        if(result < outputMin)
        {
            result = outputMin;
        }

        return result;
    }

    if(pidAlgorithm)    //if Proportional-Integral-Derivative
    {
        result = (kP*error) + (kI*integral) + (kD*derivative);

        //Make output values within output range
        if(result > outputMax)
        {
            result = outputMax;
        }
        if(result < outputMin)
        {
            result = outputMin;
        }

        return result;
    }

    if(pdAlgorithm) //if Proportional-Derivative
    {
        result = (kP*error) + (kD*derivative);

        //Make output values within output range
        if(result > outputMax)
        {
            result = outputMax;
        }
        if(result < outputMin)
        {
            result = outputMin;
        }

        return result;
    }
}

void QPID::setSetPoint(double set)
{
    setPoint = set;
}

bool QPID::getDirection()
{
    return direction;
}

double QPID::getSetPoint()
{
    return setPoint;
}

int QPID::getType()
{
    if(pAlgorithm)
    {
        return 0;
    }
    else if(piAlgorithm)
    {
        return 1;
    }
    else if(pidAlgorithm)
    {
        return 2;
    }
    else if(pdAlgorithm)
    {
        return 3;
    }
    else
    {
        return 4;   //if none are set (return error code)
    }
}
