
#include "QbApi.h"

int main(int argc, char *argv[])
{
	QbString usage = "Usage:\n";
	usage += " deleteworkerresources HOSTNAME RESOURCE_NAME [...]\n\n";
	usage += "Example: \n";
	usage += " deleteworkerresources rendernode01 host.myres host.res2\n";

	if(argc < 3) {
		cerr << "ERROR: insufficient number of args\n";
		cout << usage << endl;
		exit(1);
	}

	QbString host(argv[1]);
	QbStringList resources;
	for(int i = 2; i < argc; i++) {
		resources.push(new QbString(argv[i]));
	}
	cout << "Deleting worker resources["  << resources.join(",") <<
		"] from worker[" <<	host << "]\n";

	QB_BOOL ret = qbdeleteworkerresources(host, resources);

	if(ret) {
		cout << "SUCCESS!\n";
	} else {
		cout << "FAIL!\n";
		exit(1);
	}

	return 0;
}
