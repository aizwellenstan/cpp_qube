#include "QbApi.h"
#include <iostream.h>

//
// file: rangesubmit.cpp
//
// description: demonstrate how to submit a "cmdrange" job to the Qube
//   farm
//
// usage: rangesubmit <range> <subjobs> <command>
//
// example:
//  rangesubmit 1-100x2 10 echo QB_FRAME_NUMBER
//
int main(int argc, char *argv[])
{
	// basic sanity check
	if(argc < 4) {
		cerr << "ERROR: insufficient args\n";
		cerr << "Usage: rangesubmit <range> <subjobs> <command>\n";
		exit(1);
	}

	QbJob *job = new QbJob();

	// set the jobtype
	job->prototype("cmdrange");

	// set up agenda list for the job
	QbRange range;
	range.parse(argv[1]);
	int i;
	for(i = 0; i < range.length(); i++) {
		QbWork *work = new QbWork();
		work->name(QbString(range.get(i)));
		job->agenda().push(work);
	}	

	// set the number of "CPUs" (subjobs) that will process this job
	job->cpus(atoi(argv[2]));

	// The rest of the command line args form the actual command that
	// gets run by the cmdrange backend.  We'll put it in the
	// "package" section of the "job" object, and in particular the
	// "cmdline" package variable.

	// Join the rest of the cmdline args with space
	QbString cmdlineStr = argv[3];
	for(i = 4; i < argc; i++) {
		cmdlineStr += " ";
		cmdlineStr += argv[i];
	}

	// create the package data structure, then populate it
	qb_package* pkg = qb_package_new();
	qb_package* cmdline = qb_package_new();
	qb_package_set_name(cmdline, "cmdline");
	qb_package_set_value(cmdline, cmdlineStr.value());
	qb_package_list_push(pkg, cmdline);

	// convert the package data structure into a "packed" string, and
	// add it to the job object
	QbString pkg_data;
	pkg_data = qb_package_compact(pkg);
	job->data(pkg_data);
	qb_package_destroy(pkg);
	
	// submit the job
	QbJobList jobs;
	jobs.push(job);
	QbJobList submitted;
	if(!qbsubmit(jobs, submitted)) {
		cerr << "ERROR: submission error\n";
		exit(1);
	}
	for(i = 0; i < submitted.length(); i++) {
		cout << "job submitted: " << submitted.get(i)->id() << endl;
	}

	return 0;
}
