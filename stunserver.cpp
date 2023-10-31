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
    sock.sbind("127.0.0.1", 8080);

    Json json;

    while (1) {
        sockrecv_t myrecv = sock.srecvfrom(1024);
        JsonNode mydata = json.parse(myrecv.string);

        if (!mydata["command"].is_null()) {
            if (mydata["command"].str == "connect") {
                JsonNode node0;
                JsonNode nodetmp;

                for (sockaddress_t i : connected) {
                    JsonNode node1, node2;
                    node1.addPair("from", "stunserver");
                    node1.addPair("status", "connected");
                    node1.addPair("string", myrecv.addr.str());
                    node1.addPair("ip", myrecv.addr.ip);
                    node1.addPair("port", myrecv.addr.port);

                    node2.addPair("string", i.str());
                    node2.addPair("ip", i.ip);
                    node2.addPair("port", i.port);
                    nodetmp.arrayAppend(node2);

                    sock.ssendto(json.dump(node1), i);
                }

                node0.addPair("from", "stunserver");
                node0.addPair("clients", nodetmp);

                sock.ssendto(json.dump(node0), myrecv.addr);

                connected.push_back(myrecv.addr);
            }
            
            else if (mydata["command"].str == "disconnect") {
                connected.erase(find(connected.begin(), connected.end(), myrecv.addr));

                for (sockaddress_t i : connected) {
                    JsonNode node;
                    node.addPair("from", "stunserver");
                    node.addPair("status", "disconnected");
                    node.addPair("string", myrecv.addr.str());

                    sock.ssendto(json.dump(node), i);
                }
            }
        }

    }
}