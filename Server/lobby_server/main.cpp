#include "Server/Server.h"

using namespace realtrick;
using namespace std;

int main() {

	try {
		Server *s = new Server();
		s->run();
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}

