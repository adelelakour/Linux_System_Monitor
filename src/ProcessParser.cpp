
//
// Created by adelelakour on 10.09.24.
//

#include "ProcessParser.h"

#include <filesystem>
#include "ProcessParser.h"
#include <optional>
#include "Constants.h"
#include "util.h"
#include "std_includes.h"
#include<optional>

namespace fs=std::filesystem;

//declaration
float get_sys_active_cpu_time(vector<string> values);
float get_sys_idle_cpu_time(vector<string>values);



//cmd should be in /proc/[pid]/cmdline
string ProcessParser::getCmd(string pid) {

  string path = Path::basePath() + pid + Path::cmdPath();
  auto stream = Util::getStream(path);
  string line;
  if(getline(stream, line)) {
    return line;
  }
  return "";;
}


// save all pid in /proc
vector<string> ProcessParser::getPidList() {
  string path = "/proc";
  vector<string> list_of_ids{};

  for(const auto& item : fs::directory_iterator(path)) {
    if(std::filesystem::is_directory(item.path()) && isdigit(item.path().filename().string()[0])) {
      list_of_ids.push_back(item.path().filename().string());
    }
  }
  return list_of_ids;
}



// in proc/[PID]/status (VmData)
string ProcessParser::getVmSize(string pid) {
  string path = Path::basePath() + pid + Path::statusPath();
  auto stream = Util::getStream(path);
  string line;
  string value{};
  vector<string> words_in_lines{};

  while (getline(stream, line)) {
    if(line.find("VmData") == 0) {
      istringstream ss (line);

      istream_iterator<string> start(ss), end;
      vector<string> words_in_line(start, end);
      value = words_in_line[1];
    }
  }

  return value;
}


string ProcessParser::getProcUpTime(string pid)
{
  string line;
  string value;
  float result;
  auto stream = Util::getStream((Path::basePath() + pid + "/" +  Path::statPath()));
  getline(stream, line);
  string str = line;
  istringstream buf(str);
  istream_iterator<string> beg(buf), end;
  vector<string> values(beg, end); // done!
  // Using sysconf to get clock ticks of the host machine
  return to_string(float(stof(values[14])/sysconf(_SC_CLK_TCK)));
}


long int ProcessParser::getSysUpTime() {
  string line;
  auto stream = Util::getStream((Path::basePath() + Path::upTimePath()));
  getline(stream, line);
  istringstream buf(line);
  istream_iterator<string> beg(buf), end;
  vector<string> values(beg, end);
  return stoi(values[0]);
}



// Time Process Active on CPU / Total Running time of process
string ProcessParser::getCpuPercent(string pid)
{
  string line;
  string value;
  float result;
  auto stream = Util::getStream((Path::basePath()+ pid + "/" + Path::statPath()));
  getline(stream, line);
  istringstream ss(line);
  istream_iterator<string> start(ss), end;
  vector<string> values(start, end);

  float utime = stof(values[14]);
  float stime = stof(values[15]);
  float cutime = stof(values[16]);
  float cstime = stof(values[17]);
  float starttime = stof(values[22]);

  float uptime = ProcessParser::getSysUpTime();

  float freq = sysconf(_SC_CLK_TCK);
  float time_process_active_CPU = utime + stime + cutime + cstime;
  float Time_Process_Took_in_Sec = uptime - (starttime/freq);
  result = 100.0*((time_process_active_CPU/freq)/Time_Process_Took_in_Sec);
  return to_string(result);
}


string ProcessParser::getProcUser(string pid) {
  auto stream =
      Util::getStream((Path::basePath() + pid + "/" + Path::statusPath()));
  string line{};
  string Uid{};
  string userName{};

  while (getline(stream, line)) {
    if (line.find("Uid") == 0) {
      istringstream ss(line);
      istream_iterator<string> start(ss), end;
      vector<string> words(start, end);
      Uid = words[1];
      cout << "UID : " << Uid << endl;
      break;
    }
  }

  auto stream2 = Util::getStream("/etc/passwd");
  while (getline(stream2, line)) {
    if (line.find(Uid) != string::npos) {
      istringstream ss2(line);
      userName = line.substr(0, line.find(":"));
    }
  }
  return userName;
}



int ProcessParser::getNumberOfCores() {
  auto stream = Util::getStream((Path::basePath() + "cpuinfo"));
  string line{};
  string attribute{"cpu cores"};
  string value= "";

  while (getline(stream, line)) {
    if(line.find(attribute) != string::npos) {
      istringstream ss(line);
      istream_iterator<string> start(ss), end;
      vector<string> list_of_words(start, end);
      value = list_of_words.back();
      break;
    }
  }
  return std::stoi(value);
}



// return CPU info for the send core number since the last boot
// example to one line: cpu6 304861 118 69275 5833691 7271 0 376 0 0 0
vector<string> ProcessParser::getSysCpuPercent(string coreNumber)
{
  string line;
  string name = "cpu" + coreNumber;
  auto stream = Util::getStream((Path::basePath() + Path::statPath()));
  while (std::getline(stream, line)) {
    if (line.find(name) == 0) {
      istringstream ss(line);
      istream_iterator<string> start(ss), end;
      vector<string> values(start, end);
      return values;
    }
  }
  return (vector<string>());
}


float get_sys_active_cpu_time(vector<string> values)
{
  return (stof(values[S_USER]) +
          stof(values[S_NICE]) +
          stof(values[S_SYSTEM]) +
          stof(values[S_IRQ]) +
          stof(values[S_SOFTIRQ]) +
          stof(values[S_STEAL]) +
          stof(values[S_GUEST]) +
          stof(values[S_GUEST_NICE]));
}

float get_sys_idle_cpu_time(vector<string>values)
{
  return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}


string ProcessParser::printCpuStats(vector<string> values1, vector<string> values2)
{
  float activeTime = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);
  float idleTime = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);
  float totalTime = activeTime + idleTime;
  float result = 100.0*(activeTime / totalTime);
  return to_string(result);
}



float ProcessParser::getSysRamPercent()
{
  string line;
  string name1 = "MemAvailable:";
  string name2 = "MemFree:";
  string name3 = "Buffers:";

  string value;
  int result;
  auto stream = Util::getStream((Path::basePath() + Path::memInfoPath()));
  float total_mem = 0;
  float free_mem = 0;
  float buffers = 0;
  while (std::getline(stream, line)) {
    if (total_mem != 0 && free_mem != 0)
      break;
    if (line.compare(0, name1.size(), name1) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      total_mem = stof(values[1]);
    }
    if (line.compare(0, name2.size(), name2) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      free_mem = stof(values[1]);
    }
    if (line.compare(0, name3.size(), name3) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      buffers = stof(values[1]);
    }
  }
  //calculating usage:
  return float(100.0*(1-(free_mem/(total_mem-buffers))));
}


string ProcessParser::getSysKernelVersion()
{
  string line;
  string name = "Linux version ";
  auto stream = Util::getStream((Path::basePath() + Path::versionPath()));
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(),name) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      return values[2];
    }
  }
  return "";
}



string ProcessParser::getOsName()
{
  string line;
  string name = "PRETTY_NAME=";

  auto stream = Util::getStream(("/etc/os-release"));

  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::size_t found = line.find("=");
      found++;
      string result = line.substr(found);
      result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
      return result;
    }
  }
  return "";

}


int ProcessParser::getTotalThreads() {
  string line;
  int result = 0;
  string name = "Threads:";
  vector<string>_list = ProcessParser::getPidList();
  for (int i=0 ; i<_list.size();i++) {
    string pid = _list[i];
    //getting every process and reading their number of their threads
    auto stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while (std::getline(stream, line)) {
      if (line.compare(0, name.size(), name) == 0) {
        istringstream buf(line);
        istream_iterator<string> beg(buf), end;
        vector<string> values(beg, end);
        result += stoi(values[1]);
        break;
      }
    }
    return result;
  }
}


  int ProcessParser::getTotalNumberOfProcesses()
  {
    string line;
    int result = 0;
    string name = "processes";
    auto stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
      if (line.compare(0, name.size(), name) == 0) {
        istringstream buf(line);
        istream_iterator<string> beg(buf), end;
        vector<string> values(beg, end);
        result += stoi(values[1]);
        break;
      }
    }
    return result;
  }


  int ProcessParser::getNumberOfRunningProcesses()
  {
    string line;
    int result = 0;
    string name = "procs_running";
    auto stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
      if (line.compare(0, name.size(), name) == 0) {
        istringstream buf(line);
        istream_iterator<string> beg(buf), end;
        vector<string> values(beg, end);
        result += stoi(values[1]);
        break;
      }
    }
    return result;
  }