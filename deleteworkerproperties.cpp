
#include "QbApi.h"

int main(int argc, char *argv[])
{
	QbString usage = "Usage:\n";
	usage += " deleteworkerproperties HOSTNAME PROPERTY_NAME [...]\n\n";
	usage += "Example: \n";
	usage += " deleteworkerproperties rendernode01 host.myprop host.prop2\n";

	if(argc < 3) {
		cerr << "ERROR: insufficient number of args\n";
		cout << usage << endl;
		exit(1);
	}

	QbString host(argv[1]);
	QbStringList properties;
	for(int i = 2; i < argc; i++) {
		properties.push(new QbString(argv[i]));
	}
	cout << "Deleting worker properties["  << properties.join(",") <<
		"] from worker[" <<	host << "]\n";

	QB_BOOL ret = qbdeleteworkerproperties(host, properties);

	if(ret) {
		cout << "SUCCESS!\n";
	} else {
		cout << "FAIL!\n";
		exit(1);
	}

	return 0;
}
