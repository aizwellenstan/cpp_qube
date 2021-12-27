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

#include "QbApi.h"
#include "QbParam.h"

// Formatting routines
// ##################################################################################
QB_VOID space(QB_INT n)
{
	cout << "  ";
	while(n-- > 0) 
		cout << " ";
}

static QbString data;

const QbString& element(QbJob* job, const QbString& field) 
{
	if (field == "id") {
		data = job->id();
		return data;
	}

	if (field == "pid") {
		data = job->pid();
		return data;
	}

	if (field == "pgrp") {
		data = job->pgrp();
		return data;
	}

	if (field == "label")
		return job->label();

	if (field == "name")
		return job->name();

	if (field == "user")
		return job->user();

	if (field == "priority") {
		data = job->priority();
		return data;
	}

	if (field == "cluster")
		return job->cluster();

	if (field == "groups")
		return job->groups();

	if (field == "status") {
		data = job->status().value();
		return data;
	}

	if (field == "cpus") {
		data = job->cpustally().running();
		data += "/";
		data += job->cpus();
		return data;
	}

	if (field == "%") {
		if (job->todo())  {
			data = (job->todotally().complete() * 100) / job->todo();
		}  else {
			data = "n/a";
		}
		return data;
	}

	if (field == "type")
		return job->prototype();

	return data;
}

const QbString& element(QbJob* job, QbSubjob* subjob,  const QbString& field) 
{
	if (field == "subid") {
		data = job->id();
		data += ".";
		data += subjob->id();
		return data;
	}

	if (field == "name")
		return job->name();

	if (field == "user")
		return job->user();

	if (field == "priority") {
		data = job->priority();
		return data;
	}

	if (field == "cluster")
		return job->cluster();

	if (field == "groups")
		return job->groups();

	if (field == "host") {
		data = subjob->host(); 
		if (subjob->host().length()) {
			data += " (";
			data += subjob->address().stringvalue();
			data += ")";
		}
		return data;
	}

	if (field == "status") {
		data = subjob->status().value();
		return data;
	}

	if (field == "type")
		return job->prototype();

	return data;
}

QB_VOID display(QbJobList& list, QbQuery& query, const QbStringList& fields)
{
	if (fields.length() <= 0)
		return;

	QB_INT running_cpus = 0, total_cpus = 0, running_work = 0, total_work = 0;
	QB_INT *lengths = new QB_INT[fields.length()];
	QB_INT i;
	for (i = 0; i < fields.length(); i++)
		lengths[i] = fields.get(i)->length();

	for (i = 0; i < list.length(); i++) {
		QbJob *job = list.get(i);
		if (job == QB_NULL)
			continue;

		for (QB_INT j = 0; j < fields.length(); j++) {
			QbString ref;
			ref = element(job, *(fields.get(j)));
			lengths[j] = lengths[j] < ref.length() ? ref.length() : lengths[j];	
		}

		running_cpus += job->cpustally().running();
		total_cpus += job->cpus();

		running_work += job->todotally().running();
		total_work += job->todo();
	} 

	cout << "total: " << running_cpus << "/" << total_cpus << " cpu(s)\t";
	cout << running_work << "/" << total_work << " work" << endl;

	QB_INT width = 10;
	for (i = 0; i < fields.length() - 1; i++) {
		cout << fields.get(i)->value(); space(lengths[i] - fields.get(i)->length());
		width += lengths[i] + 1;
	}

	cout << fields.get(fields.length() - 1)->value() << endl;

	for (i = 0; i < list.length(); i++) {
		QbJob *job = list.get(i);
		if (job == QB_NULL)
			continue;

		for (QB_INT j = 0; j < fields.length(); j++) {
			QbString ref;
			ref = element(job, *(fields.get(j)));
			cout << ref.value(); space(lengths[j] - ref.length());
		}

		cout << endl;
	} 

	delete [] lengths;
}

// ##################################################################################

int main(QB_INT argc, QB_CHAR** argv)
{
	/* 
	 	define options 
		these routines are just Qube! command line parameter handling helpers
	 */
	QbParam param;
	param.defineStringOption(QbString("user"));

	if (!param.parse(argc, argv)) {
		cout << param.error().value();
		exit(1);
	}

	/* 
		here we setup the job id filter.  The filter's name is 'id' and the values
	        are added in to the 'fields' object.	
	 */
	QbQuery query;
	if (param.length() > 0) {
		QbFilter* filter = new QbFilter(QbString("id"));
		for(QB_INT i = 0; i < param.length(); i++) {
			filter->fields().push(new QbField(param.arg(i).integervalue()));
		}
		query.filters().push(filter);
	}

	/*
		here we setup the job user filter.  If the user's name is 'all' then we 
	       use a parser routine (in QbApi.h) to auto-magically parse comma delimited
       		usernames and place them in a Filter with Fields.	       

		else... we assume it's your jobs we're querying for and just place the user
		as the current user's name.
	 */
	QbString user;
	QbFilter *userFilter = QB_NULL;
	if (param.get(QbString("user"), user)) {
		if (user != "all")
			userFilter = qbfilterparam(QbString("user"), user);
			
	} else {
		if (param.length() == 0) {
			userFilter = new QbFilter(QbString("user"));
			userFilter->fields().push(new QbField(qbcurrentuser()));
		}
	}

	/* 
		we pass the query object into the qbjobinfo routine and
	       the result if successful will be placed in the 'jobs' object.

		we return a QB_TRUE on success and QB_FALSE on an error.	       
	 */
	QbJobList jobs;
	if (!qbjobinfo(query, jobs)) {
		cout << "ERROR: couldn't obtain data from supervisor." << endl;
		exit(1);
	}

	/* 
	
	  	Format the data and display  
	
	 */
	QbStringList fields;
	fields.push(new QbString("%"));
	fields.push(new QbString("id"));
	fields.push(new QbString("pid"));
	fields.push(new QbString("pgrp"));
	fields.push(new QbString("label"));
	fields.push(new QbString("status"));
	fields.push(new QbString("user"));
	fields.push(new QbString("type"));
	fields.push(new QbString("name"));
	fields.push(new QbString("cpus"));
	fields.push(new QbString("priority"));
	fields.push(new QbString("cluster"));
	fields.push(new QbString("groups"));

	display(jobs, query, fields);

	return 0;
}


/*  these are to satisfy the QbParam object which automatically assumes these 2 functions are linked  */
void display_help(){}
void display_version() {}


