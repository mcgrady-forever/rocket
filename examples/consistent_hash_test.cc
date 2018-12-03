#include <string>

#include "base/consistent_hash.h"

using namespace rocket::base;
using namespace std;

struct Node
{
	string _name;
	Node(string name) : _name(name) {}
};

int main(int argc, char const *argv[])
{
	std::vector<Node> v = {
		Node("node1"),
		Node("node2"),
		Node("node3"),
		Node("node4")
	};
}