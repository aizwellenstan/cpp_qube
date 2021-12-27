
#include "QbApi.h"

int main(int argc, char *argv[])
{
	QbString usage = "Usage:\n";
	usage += " updateworkerresources HOSTNAME RESOURCES\n\n";
	usage += "Example: \n";
	usage += " updateworkerresources rendernode01 host.myres=3/10,host.res2=1/5\n";

	if(argc != 3) {
		cerr << "ERROR: incorrect number of args\n";
		cout << usage << endl;
		exit(1);
	}

	QbString host(argv[1]);
	QbString resources(argv[2]);
	cout << "Setting worker resources["  << resources << "] on worker[" <<
		host << "]\n";

	QB_BOOL ret = qbupdateworkerresources(host, resources);

	if(ret) {
		cout << "SUCCESS!\n";
	} else {
		cout << "FAIL!\n";
		exit(1);
	}

	return 0;
}
