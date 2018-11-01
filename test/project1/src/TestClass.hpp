#pragma once

#include <string>

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

    // constructor should be ignored
    TestClass();

    // KC-1-2
    void onlyIn(int, double, const std::string&, std::string, const double&, bool);
    void onlyOut(int&, double&, std::string&, double&, bool&);
    void inThenOut(
        int, double, const std::string&, std::string,
        int&, double&, std::string&);
    void outThenIn(
        int&, double&, std::string&,
        int, double, const std::string&, std::string);
    void inOutIn(
        int, double, const std::string&, std::string,
        int&, double&, std::string&,
        const std::string&);
    void outInOut(
        int&, double&, std::string&,
        std::string, int, double, bool,
        double&);

};