//
//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;

Resistor** resistors =
    nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

string validCommands[7] = {
    "maxVal", "insertR", "modifyR", "printR", "deleteR", "setV", "solve",
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);
void handleSolve();

bool checkEOF(stringstream& ss);
bool checkValidArg(stringstream& ss);
bool contains(string command);
bool checkSameName(string Name);
bool checkNameFound(string name);
bool checkConnectSameNode(int node1, int node2);
bool checkNodeValueRange(int node);
bool checkNegRes(double res);
bool checkNameKeyword(string name);

//MAIN
int main() {
  string line;
  // TODO: Implement the main function here

  bool checker = true;
  while (checker) {
    cout << ">>> ";
    cout.flush();
    string command;
    getline(cin, line);
    line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); //This line is from a Piazza post, from Vir Patel, on Piazza post @569
    stringstream ss(line);
    ss >> command;

    if (cin.eof()) {
      break;
    }

    if (!(contains(command))) {
      cout << "Error: invalid command" << endl;
      continue;
    }

    if (command == "maxVal") {
      handleMaxVal(ss);
    }

    if (command == "insertR") {
      handleInsertR(ss);
    }

    if (command == "modifyR") {
      handleModifyR(ss);
    }

    if (command == "printR") {
      handlePrintR(ss);
    }

    if (command == "deleteR") {
      handleDeleteR(ss);
    }

    if (command == "setV") {
      handleSetV(ss);
    }

    if (command == "solve") {
      handleSolve();
    }
  }

  for (int i = 0; i < maxResistors; i++) {
    delete resistors[i];
    resistors[i] = nullptr;
  }

  delete[] resistors;
  resistors = nullptr;

  delete[] nodes;
  nodes = nullptr;
}

// TODO: Implement functions here

void handleMaxVal(stringstream& ss) {
  int newMaxNodeNumber, newMaxResistors;

  ss >> newMaxNodeNumber;
  if (!checkEOF(ss)) {
    return;
  }
  if (!checkValidArg(ss)) {
    return;
  }

  ss >> newMaxResistors;
  if (!checkValidArg(ss)) {
    return;
  }

  // if (resistors != nullptr || nodes != nullptr) {
  //   for (int i = 0; i < maxResistors; i++) {
  //     delete resistors[i];
  //     resistors[i] = nullptr;
  //   }
  //   delete[] resistors;
  //   resistors = nullptr;

  //   delete[] nodes;
  //   nodes = nullptr;
  // }

  if (resistors != nullptr) {
    for (int i = 0; i < maxResistors; i++) {
      if (resistors[i] != nullptr) {
        delete resistors[i];     // Delete each resistor object
        resistors[i] = nullptr;  // Set pointer to nullptr
      }
    }
    delete[] resistors;   // Delete the array of pointers
    resistors = nullptr;  // Set pointer to nullptr
  }

  if (nodes != nullptr) {
    delete[] nodes;   // Delete the array of nodes
    nodes = nullptr;  // Set pointer to nullptr
  }

  maxNodeNumber = newMaxNodeNumber;
  maxResistors = newMaxResistors;

  // Resistor** resistors =
  //     nullptr;  // Pointer that should point to an array of Resistor pointers
  // Node* nodes = nullptr;
  resistors = new Resistor*[maxResistors];  // Use the global variable
  for (int i = 0; i < maxResistors; i++) {
    resistors[i] = nullptr;  // Initialize each pointer to nullptr to avoid
                             // dangling pointers
  }

  nodes = new Node[maxNodeNumber];  // Use the global variable

  resistorsCount = 0;

  cout << fixed << setprecision(2);
  cout << "New network: max node number is " << maxNodeNumber
       << "; max resistors is " << maxResistors << endl;
}

void handleInsertR(stringstream& ss) {
  string name;
  double resistance;
  int nodeid1, nodeid2;

  ss >> name;
  if (!checkEOF(ss)) {
    return;
  }
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNameKeyword(name)) {
    return;
  }
  if (!checkSameName(name)) {
    return;
  }

  ss >> resistance;
  if (!checkEOF(ss)) {
    return;
  }
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNegRes(resistance)) {
    return;
  }

  ss >> nodeid1;
  if (!checkEOF(ss)) {
    return;
  }
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNodeValueRange(nodeid1)) {
    return;
  }

  ss >> nodeid2;
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNodeValueRange(nodeid2)) {
    return;
  }
  if (!checkConnectSameNode(nodeid1, nodeid2)) {
    return;
  }

  int endpoints[2] = {nodeid1 - 1, nodeid2 - 1};
  resistors[resistorsCount] = new Resistor(name, resistance, endpoints);

  nodes[nodeid1 - 1].addResistor(resistorsCount);
  nodes[nodeid2 - 1].addResistor(resistorsCount);
  resistorsCount++;

  cout << fixed << setprecision(2);
  cout << "Inserted: resistor " << name << " " << resistance << " Ohms "
       << nodeid1 << "->" << nodeid2 << endl;
}

void handleModifyR(stringstream& ss) {
  string name;
  double resistance;

  ss >> name;
  if (!checkEOF(ss)) {
    return;
  }
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNameKeyword(name)) {
    return;
  }
  if (!checkNameFound(name)) {
    return;
  }

  ss >> resistance;
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNegRes(resistance)) {
    return;
  }

  for (int i = 0; i < maxResistors; i++) {
    if (resistors[i] != nullptr && resistors[i]->getName() == name) {
      double oldResistance = resistors[i]->getResistance();
      resistors[i]->setResistance(resistance);
      cout << fixed << setprecision(2);
      cout << "Modified: resistor " << name << " from " << oldResistance
           << " Ohms to " << resistors[i]->getResistance() << " Ohms" << endl;
    }
  }
}

void handlePrintR(stringstream& ss) {
  string name;

  ss >> name;
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNameFound(name)) {
    return;
  }

  cout << "Print:" << endl;
  for (int i = 0; i < maxResistors; i++) {
    if (resistors[i] != nullptr && resistors[i]->getName() == name) {
      resistors[i]->print();
    }
  }
}

void handleDeleteR(stringstream& ss) {
  string keyword;
  ss >> keyword;
  if (!checkValidArg(ss)) {
    return;
  }

  for (int i = 0; i < maxResistors; i++) {
    delete resistors[i];
    resistors[i] = nullptr;
  }

  delete[] nodes;
  nodes = nullptr;
  nodes = new Node[maxNodeNumber];

  cout << "Deleted: all resistors" << endl;
}

void handleSetV(stringstream& ss) {
  int nodeid;
  double voltage;

  ss >> nodeid;
  if (!checkEOF(ss)) {
    return;
  }
  if (!checkValidArg(ss)) {
    return;
  }
  if (!checkNodeValueRange(nodeid)) {
    return;
  }

  ss >> voltage;
  if (!checkValidArg(ss)) {
    return;
  }

  nodes[nodeid - 1].setVoltage(voltage);

  cout << fixed << setprecision(2);
  cout << "Set: node " << nodeid << " to " << voltage << " Volts" << endl;
}

void handleSolve() {
  
  for (int i = 0; i < maxNodeNumber; i++) {
    if (!(nodes[i].isVoltageSet())) {
      nodes[i].setVoltageInternally(0);
    }
  }

  bool checker = true;
  while (checker) {
    checker = false;

    for (int i = 0; i < maxNodeNumber; i++) {
      if (!(nodes[i].isVoltageSet())) {
          //For every sad node, we get the resIDArray of it. This contains all the indices of all the resistors 
          //attached to the sad node. From there, we set up reciprocalSum and weightedSum. Then we set up a loop
          // to go through the resIDArray. For each element of the resIDArray, its element value gives us a index
          // of a resistor, in the main global resistors array. We use this index to get access to the resistor 
          // itself. From there, we use can take its resistance. Then, we can also use getOtherEndpoint on this
          // resistor. Note, this resistor is already attaced to our sad node, i. So argument to get other endpoint
          // will be i. Now this returns the index of the other node its connected to. Now use this node to index
          // global nodes[]. Use .getVoltage on the node object to get its voltage. Equipped with the resistance 
          // and the other node voltage, we can add to the weighted sum and reciprocal sum of the SAD NODE.
          // Then we can calculate our A (new voltage value). Do a quick check to see if the difference is 
          // greater than the min diff. If it is, set checker to true so we can loop again. Then set the internal
          // voltage as A for the sad node, and move on to the next sad node. After all sad nodes, checker
          // will either be true or false. If false, done. Else, repeat.
          int* resArr = nodes[i].getResIDArray();
          int resCount = nodes[i].getNumRes();
          double reciprocalSum = 0.0;
          double weightedSum = 0.0;

          for (int j = 0; j < resCount; j++) {
            int indexInGlobalResistorsArr = resArr[j];
            int otherEndpoint = resistors[indexInGlobalResistorsArr]->getOtherEndpoint(i);

            double Ri = resistors[indexInGlobalResistorsArr]->getResistance();
            double Vi = nodes[otherEndpoint].getVoltage();
            
            reciprocalSum += 1.0 / Ri;
            weightedSum += Vi / Ri;
          }

          double A = (1.0 / reciprocalSum) * weightedSum;
          if ((A - nodes[i].getVoltage()) > 0.0001) {
            checker = true;
          }
          nodes[i].setVoltageInternally(A);
        
      }
    } 
  }

  for (int i = 0; i < maxNodeNumber; i++) {
    if (nodes[i].getNumRes() > 0) {
      cout << fixed << setprecision(2);
      cout << "Node " << (i + 1) << ": " << nodes[i].getVoltage() << " V" << endl;
    }
  }

}
// CHECKER
// FUNCTIONS/////////////////////////////////////////////////////////////////////
bool checkEOF(stringstream& ss) {
  if (ss.eof()) {
    cout << "Error: too few arguments" << endl;
    return false;
  }
  return true;
}

bool checkValidArg(stringstream& ss) {
  if (ss.fail()) {
    cout << "Error: invalid argument" << endl;
    return false;
  }
  return true;
}

bool contains(string command) {
  for (int i = 0; i < 7; i++) {
    if (validCommands[i] == command) {
      return true;
    }
  }
  return false;
}

bool checkNameKeyword(string name) {
  if (name == "all") {
    cout << "Error: resistor name cannot be keyword \"all\"" << endl;
    return false;
  }
  return true;
}

bool checkNegRes(double res) {
  if (res < 0.0) {
    cout << "Error: negative resistance" << endl;
    return false;
  }
  return true;
}

bool checkNodeValueRange(int node) {
  if (node < 1 || node > maxNodeNumber) {
    cout << "Error: node value is out of permitted range 1 - " << maxNodeNumber
         << endl;
    return false;
  }
  return true;
}

bool checkConnectSameNode(int node1, int node2) {
  if (node1 == node2) {
    cout << "Error: both terminals of resistor connect to same node" << endl;
    return false;
  }
  return true;
}

bool checkNameFound(string name) {
  for (int i = 0; i < maxResistors; i++) {
    if (resistors[i] != nullptr && (*(resistors[i])).getName() == name) {
      return true;
    }
  }
  cout << "Error: resistor " << name << " not found" << endl;
  return false;
}

bool checkSameName(string Name) {
  for (int i = 0; i < maxResistors; i++) {
    if (resistors[i] != nullptr && (*(resistors[i])).getName() == Name) {
      cout << "resistor name already exists"
           << endl;  // equal to resistors[i] -> getName()
      return false;
    }
  }
  return true;
}