//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: 
// Date:

//   Node();  // sets numRes to 0, voltage to 0 and voltageIsSet to false
//   bool canAddResistor();  // returns if the number of resistors is less than
//                           // MAX_RESISTORS_PER_NODE
//   void addResistor(int rIndex);  // adds a resistor to resIDArray array and
//                                  // increments number of resistors

//   double getVoltage();  // returns voltage
//   void setVoltage(
//       double voltage_);  // sets voltage and sets voltageIsSet to true
//   void setVoltageInternally(
//       double voltage_);  // sets voltage temporarily during iterations for solve
//                          // command
//   bool isVoltageSet();   // returns voltageIsSet

//   int getNumRes();       // returns numRes
//   int* getResIDArray();  // returns resIDArray

#include "Node.h"
#include <iomanip>

Node::Node() {
  numRes = 0;
  voltage = 0;
  voltageIsSet = false;
}

bool Node::canAddResistor() {
  return numRes < MAX_RESISTORS_PER_NODE; 
}


void Node::addResistor(int rIndex) {
    if (canAddResistor()) {
        resIDArray[numRes] = rIndex;
        numRes++;
    }
}

double Node::getVoltage() {
    return voltage;
}

void Node::setVoltage(double voltage_) {
    voltage = voltage_;
    voltageIsSet = true;
}

void Node::setVoltageInternally(double voltage_) {
    voltage = voltage_;
}


bool Node::isVoltageSet() {
    return voltageIsSet;
}

int Node::getNumRes() {
    return numRes;
}

int* Node::getResIDArray() {
    return resIDArray;
}



