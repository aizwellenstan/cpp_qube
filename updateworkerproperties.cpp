
#include "QbApi.h"

int main(int argc, char *argv[])
{
	QbString usage = "Usage:\n";
	usage += " updateworkerproperties HOSTNAME PROPERTIES\n\n";
	usage += "Example: \n";
	usage += " updateworkerproperties rendernode01 host.myprop=foobar,host.prop2=3.141592\n";

	if(argc != 3) {
		cerr << "ERROR: incorrect number of args\n";
		cout << usage << endl;
		exit(1);
	}

	QbString host(argv[1]);
	QbString properties(argv[2]);
	cout << "Setting worker properties["  << properties << "] on worker[" <<
		host << "]\n";

	QB_BOOL ret = qbupdateworkerproperties(host, properties);

	if(ret) {
		cout << "SUCCESS!\n";
	} else {
		cout << "FAIL!\n";
		exit(1);
	}

	return 0;
}
