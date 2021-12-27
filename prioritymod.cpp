#include <stdlib.h>
#include <iostream.h>
#include <QbApi.h>

//
// file: prioritymod.cpp
//
// description: demonstrates how to modify the priority of a job
//   that's already in the system.
//
// usage: prioritymod <priority> <jobID> [<jobid> ...]
//
// example:
//  rangesubmit 1-100x2 10 echo QB_FRAME_NUMBER
//
int main(int argc, char *argv[])
{
	// basic sanity check
	if(argc < 3) {
		cerr << "ERROR: insufficient number of args\n";
		cerr << "Usage: " << argv[0] << " <priority> <jobID> [<jobID>...]\n";
		exit(1);
	}

	// get the new priority from the command line
	int newpriority = atoi(argv[1]);
	
	// set up a QbModifyCommand object
	QbModifyCommand cmd;
	QbField *field = new QbField("priority");
	cmd.fields().push(field);

	// get jobID(s) from the command line
	for(int i = 2; i < argc; i++) {
		cmd.jobids().push(new QbJobId(atoi(argv[i])));
	}

	// set up a "QbJob" object "newjob" which is used to replace the
	// old job parameters in the system
	QbJob newjob;
	newjob.priority(newpriority);
	cmd.job(newjob);

	QbJobIdList jobIDs;
	// now send a modify request to the supervisor!
	if (!qbmodify(cmd, jobIDs)) {
		cerr << "ERROR: couldn't send command to supervisor." << endl;
		exit(1);
	}

	// print out jobIDs
	for(int i = 0; i < jobIDs.length(); i++) {
		cout << "modified job[" << jobIDs.get(i)->id() << "]\n";
	}
}
