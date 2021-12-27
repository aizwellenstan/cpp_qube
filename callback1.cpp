/*
#####################################################################################
#
# Licensed Materials - Property of PipelineFX L.L.C.
#
# (C) COPYRIGHT PipelineFX Limited Liability Corporation. 2007
# All Rights Reserved.
# 				
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with 
# PipelineFX L.L.C.
#
#####################################################################################
*/

#include "QbDatatype.h"
#include "QbParam.h"
#include "QbApi.h"
#include "qbpackage.h"

#ifdef QB_HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef QB_HAVE_CONIO_H
#include <conio.h>
#endif


/*

  This programs is an example of how to setup 2 jobs and have the second depend on the first.

	Note: all of Qube!'s routines manage the memory involved with 'push' or 'shift' into a 
       	Qube! object.  In both the C and C++ Qube! routines.  Statically declaring variables
	then pushing in the pointer will cause Segfaults because Qube!'s container objects
	automatically de-allocate objects contained in them.	
*/

int main(QB_INT argc, QB_CHAR** argv)
{
	/*
	
	  First we setup the object for the first job
	
	*/
	QbJob *jobobj1 = new QbJob();

	/* we'll name the first job.... "first job". */
	jobobj1->name("first job");

	/* set the priority and cluster (not required) */
	jobobj1->priority(10);
	jobobj1->cluster("/");

	/* set the job type */
	jobobj1->prototype("cmdline");

	/* setup the package data specific to the 'cmdline' job type */
		/*    first we create a package object (this part is mostly C not C++) */
		qb_package* job1 = qb_package_new();

		/*    we then create an entry into the root package object */
		qb_package* cmdline1 = qb_package_new();

		/*    we setup the entry's data, name = cmdline, value = sleep 0 */
		qb_package_set_name(cmdline1, "cmdline");
		qb_package_set_value(cmdline1, "sleep 0");

		/*    then we push the entry into the root package object */
		qb_package_list_push(job1, cmdline1);

		/*    we convert the whole structure into a single string */
		QbString pack1 = qb_package_compact(job1);
		/* then we delete the old package structure just to be clean */
		qb_package_destroy(job1);

	/* place the string into the job's data field */
	jobobj1->data(pack1);

	/*
	
	 This is where the fun begins... we first assign a label to the job.
	 a label is used to identify jobs in the same process group without
	 using the process id's.  If you are thinking about what the 
	 supervisor does if you have the same label in the same process group...
	 it won't happen.  The supervisor enforces unique labels in the same
	 process group upon submission.
	
	 The reason we use labels is to keep things abstract. Is to keep the
	 job submission abstract.  Using job id's as the principal method
	 of linking jobs together forces the developer to create jobs which
	 don't template very well.  It also makes bi-directional callbacks
	 impossible.  
	
	*/

	jobobj1->label("first");

	/*

	  Next, we setup the hash for the second job.

	*/

	QbJob *jobobj2 = new QbJob();

	/* we'll name the second job.... "second job". */
	jobobj2->name("second job");

	/* set the priority and cluster (not required) */
	jobobj2->priority(10);
	jobobj2->cluster("/");

	/* set the job type */
	jobobj2->prototype("cmdline");

	/* setup the package data specific to the 'cmdline' job type */
		/*    first we create a package object (this part is mostly C not C++) */
		qb_package* job2 = qb_package_new();

		/*    we then create an entry into the root package object */
		qb_package* cmdline2 = qb_package_new();

		/*    we setup the entry's data, name = cmdline, value = sleep 0 */
		qb_package_set_name(cmdline2, "cmdline");
		qb_package_set_value(cmdline2, "sleep 0");

		/*    then we push the entry into the root package object */
		qb_package_list_push(job2, cmdline2);

		/*    we convert the whole structure into a single string */
		QbString pack2 = qb_package_compact(job2);
		/* then we delete the old package structure just to be clean */
		qb_package_destroy(job2);

	/* place the string into the job's data field */
	jobobj2->data(pack2);

	/*	
	  because we don't want the job to start right away, we must
	  submit it as blocked
	*/	
	jobobj2->status(QB_STATUS_BLOCKED);

	/*
	
 	  ok... so we've setup the first job's label.  we can now attach a 
	   callback to the second job so that it watches the first job. 
	
	  all triggers are identified with the following format:
	 	<status/name>-<type>-<label> 
	
	 	complete-job-first

		in the callback we are setting up, the plan is to unblock 
		the second job.		

		to do this we will use the supervisor's api call:
			qb::unblock in combination with the 
			automatically defined variable $qb::jobid.
	
	*/


	QbCallback* callback = new QbCallback();
	callback->triggers("done-job-first");
	callback->language(QB_CALLBACK_PERL);

	/*
	 
		The following "code" is really this (just escaped for C++ strings):

			print "$qb::jobid running callback\n";
			qb::unblock($qb::jobid);
	 
	 */
	callback->code("print \"$qb::jobid running callback\\n\";\nqb::unblock($qb::jobid);");
	jobobj2->callbacks().push(callback);
		


	/* execute job removal */
	QbJobList jobs;
	jobs.push(jobobj1);
	jobs.push(jobobj2);

	QbJobList result;
	qbsubmit(jobs, result);
	
	for (QB_INT i = 0; i < result.length(); i++)
		cout << "submitted: " << result.get(i)->id() << endl;

	return 0;
}


