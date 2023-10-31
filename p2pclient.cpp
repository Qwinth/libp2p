#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "../cpplibs/ssocket.hpp"
#include "../cpplibs/libjson.hpp"
using namespace std;

vector<sockaddress_t> connected;

int main() {
    SSocket sock(AF_INET, SOCK_DGRAM);
    Json json;

    JsonNode node;
    node.addPair("command", "connect");

    sock.ssendto(json.dump(node), "127.0.0.1", 8080);

    sockrecv_t myrecv = sock.srecvfrom(1024);

    cout << myrecv.string << endl;

    node.addPair("command", "disconnect");
    sock.ssendto(json.dump(node), "127.0.0.1", 8080);
    sock.sclose();
}