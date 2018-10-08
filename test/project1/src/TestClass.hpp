#pragma once

class TestClass
{
public:

    // KC-1-1
    void camelCase();
    void PascalCase();
    void snake_case();
    int  iHungarian();

    // KCE-1-1
    void sendMsg();
    void getDesc();
    void addLoc();
    void msg();
    void msgDigest(); // this will currently not be caught

    // KM-1-1
    void shortFn();
    void normalLength();
    void methodWithAveryLongNameThatIsDifficultToRemember();

    // KM-1-2
    void zeroArgs();
    void oneArg(int);
    void threeArgs(int, double, bool);
    void sixArgs(int, double, bool, int, double, bool);
    void nineArgs(int, double, bool, int, double, bool);

    // KM-1-3
    void twoConsArg(int, int);
    void threeConsArgs(int, int, int);
    void fourConsArgs(int, int, int, int);
    void fiveConsArgs(int, int, int, int, int);
    void sixConsArgsBroken(int, int, int, double, int, int);
    void twiceFiveConsArgs(int, int, int, int, int,
        double, double, double, double, double);


};