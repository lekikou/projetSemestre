#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Dag.h"

using namespace std;

Dag::Dag(){
}

/**
 * Search a node in the dag
 * @param id of node
 * @return int pos in the dag
 */
int Dag::searchNodeIntoDag(int id) {
    for (int i = 0; i < nodes.size(); ++i) {
        if(nodes[i].id == id){
            return i;
        }
    }
    return -1;
}

/**
 * Create a dag with the format file .dot
 * @param filename name of file
 */
void Dag::createDag(string filename){
    ifstream file(filename);
    string line;
    int cpt = 0;
    int posFather;
    int posSon;
    while (getline(file, line)) {
        if(cpt && line != "}") {
            std::istringstream iss(line);
            int father, son;
            string separator;
            if (iss >> son >> separator >> father && separator == " -> ") {
                cout << "error" << cpt << endl;
                break;
            } // error
            posFather = searchNodeIntoDag(father);
            if(posFather != -1){
                nodes[posFather].children.push_back(son);
                //cout << "Add" << endl;
            }else{
                Node newNode(father);
                newNode.children.push_back(son);
                nodes.push_back(newNode);
                //cout << "Create" << endl;
            }
            //cout << father << " " << son << endl;

            posSon = searchNodeIntoDag(son);
            if(posSon != -1){
                nodes[posSon].fathers.push_back(father);
                //cout << "Add" << endl;
            }else{
                Node newNode(son);
                newNode.fathers.push_back(father);
                nodes.push_back(newNode);
                //cout << "Create" << endl;
            }
        }
        cpt++;
    }
}

/**
 * Add the cost of the node into dag
 * @param filename name of file with the format .dot
 */
void Dag::addCost(string filename){
    ifstream file(filename);
    string line;
    int cpt = 0;
    int pos;
    while (getline(file, line)) {
        if(cpt) {
            istringstream iss(line);
            int id;
            double time;
            string separator;
            if (iss >> id >> separator >> time && separator == ", ") {
                cout << "error" << cpt << endl;
                break;
            } // error
            pos = searchNodeIntoDag(id);
            if(pos != -1){
                nodes[pos].cost = time;
            }else{
                cout << "error unknown id in addCost" << id << endl;
            }
            //cout << id << " " << time << endl;
        }
        cpt++;
    }
}

/**
 * When a node finish return the new node who is ready
 * @param id node
 * @return vector<int> vector of node ready to execute
 */
vector<int> Dag::nodeHandle(int id){
    vector<int> nodesReady;
    int pos = searchNodeIntoDag(id);
    int posChild;
    for(int i = 0; i < nodes[pos].children.size(); i++){
        posChild = searchNodeIntoDag(nodes[pos].children[i]);
        nodes[posChild].cptReady++;
        if(nodeReady(nodes[posChild].id)){
            nodesReady.push_back(nodes[posChild].id);
        }
    }
    return nodesReady;
}

/**
 * Test if node is ready to execute
 * @param id node
 * @return bool if ready or not
 */
bool Dag::nodeReady(int id){
    int pos = searchNodeIntoDag(id);
    if(nodes[pos].cptReady == nodes[pos].fathers.size()) {
        return true;
    }
    return false;
}

/*
 * Reset a dag
 */
void Dag::resetDag() {
    for(int i = 0; i < nodes.size(); i++){
        nodes[i].cptReady = 0;
    }
}

/**
 * Display dag
 */
void Dag::displayDag() {
    for (int i = 0; i < nodes.size(); ++i) {
        nodes[i].displayNode();
    }
}
