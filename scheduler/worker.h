/***************************************************************************
* ALPS++/scheduler library
*
* scheduler/worker.h   A class to store parameters
*
* $Id$
*
* Copyright (C) 1994-2003 by Matthias Troyer <troyer@itp.phys.ethz.ch>,
*
* Permission is hereby granted, free of charge, to any person or organization 
* obtaining a copy of the software covered by this license (the "Software") 
* to use, reproduce, display, distribute, execute, and transmit the Software, 
* and to prepare derivative works of the Software, and to permit others
* to do so for non-commerical academic use, all subject to the following:
*
* The copyright notice in the Software and this entire statement, including 
* the above license grant, this restriction and the following disclaimer, 
* must be included in all copies of the Software, in whole or in part, and 
* all derivative works of the Software, unless such copies or derivative 
* works are solely in the form of machine-executable object code generated by 
* a source language processor.

* In any scientific publication based in part or wholly on the Software, the
* use of the Software has to be acknowledged and the publications quoted
* on the web page http://www.alps.org/license/ have to be referenced.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
**************************************************************************/

#ifndef ALPS_SCHEDULER_WORKER_H
#define ALPS_SCHEDULER_WORKER_H

#include <alps/scheduler/info.h>
#include <alps/alea.h>
#include <alps/osiris.h>
#include <alps/parameters.h>
#include <boost/smart_ptr.hpp>
#include <boost/random.hpp>
#include <boost/filesystem/path.hpp>

namespace alps {

namespace scheduler {

namespace {
  const float MCTimeFactor = 0.05;
}


//=======================================================================
// AbstractWorker
//
// the abstract base class for all classes describing a Monte Carlo run
// a run is an actual simulation, running on one CPU.
// the collection of all runs with the same parameters is called
// simulation.
//-----------------------------------------------------------------------

class AbstractWorker
{
public:		
  AbstractWorker() {};
  virtual ~AbstractWorker() {};
  virtual void save_to_file(const boost::filesystem::path&) const=0;
  virtual void load_from_file(const boost::filesystem::path&)=0;
  virtual void set_parameters(const Parameters& parms)=0;
  virtual TaskInfo get_info() const = 0;
  virtual double work_done() const =0;
  virtual void start_worker() = 0;
  virtual void halt_worker() = 0;
  virtual bool handle_message(const Process& runmaster,int32_t tag) =0;
};

//=======================================================================
// Worker
//
// base class for the actual run class which will be implemented by the
// user.
//-----------------------------------------------------------------------


class Worker : public AbstractWorker
{
public:
  Worker(const ProcessList&,const Parameters&, int32_t);
  virtual ~Worker();
  void set_parameters(const Parameters& parms);
  virtual bool change_parameter(const std::string& name, const StringValue& value);
  virtual void save_worker(ODump&) const;
  virtual void load_worker(IDump&);
  virtual void write_xml(const boost::filesystem::path& name, const boost::filesystem::path& ckpt_name="") const;
  void save_to_file(const boost::filesystem::path&) const;
  void load_from_file(const boost::filesystem::path&);
  // creates a new information object containing information about this run
  TaskInfo get_info() const;
  void start_worker();
  void halt_worker();
  virtual void start() {}
  virtual void halt() {}
  virtual std::string work_phase();
  void change_phase(const std::string&);
  virtual void run();
  bool handle_message(const Process& runmaster,int32_t tag);
  virtual void dostep();
  double work_done() const;
protected:
  int32_t version;
  int32_t user_version;
  typedef boost::lagged_fibonacci607 random_type;
  //typedef boost::mt19937 random_type;
  random_type random;
  boost::variate_generator<random_type&,boost::uniform_real<> > random_01;
  double random_real(double a=0., double b=1.) 
  { return boost::variate_generator<random_type&,boost::uniform_real<> >(random,boost::uniform_real<>(a,b))();}
  int random_int(int a, int b) 
  { return boost::variate_generator<random_type&,boost::uniform_int<> >(random,boost::uniform_int<>(a,b))();}
  int random_int(int n) 
  { return boost::variate_generator<random_type&,boost::uniform_int<> >(random,boost::uniform_int<>(0,n-1))();}

  
  int node;
  Parameters parms;
  ProcessList where;
private:
  TaskInfo info;
  int halted,started;
  float stepspersec;
};


//=======================================================================
// RemoteWorker
//
// this class is just a class for proxy objects. The actual run is on
// another node/computer, messages are sent to the actual object
// and the return message relayed to the caller.
// Allows for transparent access to remote objects
//-----------------------------------------------------------------------

class RemoteWorker : public AbstractWorker
{
public:
  // constructors/destructor also constructs/destroys actual object
  RemoteWorker(const ProcessList&,const Parameters&, int32_t=0);
  virtual ~RemoteWorker();

  void set_parameters(const Parameters& parms);

  // save function also saves actual object
  void save_to_file(const boost::filesystem::path&) const;
  void load_from_file(const boost::filesystem::path&);
  void start_worker();
  void halt_worker();
  
  virtual TaskInfo get_info() const;
  double work_done() const;
  const Process& process() const {return where;}
  bool handle_message(const Process& runmaster,int32_t tag);
private:
  Process where;
};

} // namespace scheduler

} // namespace alps

#endif 
