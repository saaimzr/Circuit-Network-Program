
#include "Resistor.h"

#include <iomanip>

void Resistor::print() {
  cout << std::left << std::setw(20) << name << std::right << std::setw(8)
       << std::fixed << std::setprecision(2) << resistance << " Ohms "
       << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1] << endl;
}

  // Resistor(string name_, double resistance_,
  //          int endpoints_[2]);  // sets name, resistance and endpoint node IDs
  //   double getResistance();                  // returns the resistance
  // void setResistance(double resistance_);  // sets the resistance

  // void print();  // prints resistor info

  // int getOtherEndpoint(
  //     int nodeIndex);  // returns the other node ID that the
  //                      // resistor is attached to (other than nodeIndex)

  Resistor::Resistor(string name_, double resistance_, int endpoints_[2]) {
    name = name_;
    resistance = resistance_;
    endpointNodeIDs[0] = endpoints_[0];
    endpointNodeIDs[1] = endpoints_[1];
  }

  string Resistor::getName() {
    return name;
  }

  double Resistor::getResistance() {
    return resistance;
  }

  void Resistor::setResistance(double resistance_) {
    resistance = resistance_;
  }

  int Resistor::getOtherEndpoint(int nodeIndex) {
    if (nodeIndex == endpointNodeIDs[0]) {
      return endpointNodeIDs[1];
    } else {
      return endpointNodeIDs[0];
    }
  }
