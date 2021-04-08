#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <pistache/endpoint.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <set>
#include <nlohmann/json.hpp>
#include <string> 
#include "omp.h"


using namespace Pistache;
using json = nlohmann::json;



class TestHandler : public Http::Handler {
  HTTP_PROTOTYPE(TestHandler)
  
  void onRequest(const Http::Request& req, Http::ResponseWriter writer) override{
    try {
      json_obj_ = json::parse(req.body());
    } catch (std::exception & e) { // exception of parsing input json
      std::string errMsgStr(e.what());
      errMsgStr = "parsing json input error: " + errMsgStr;
      std::cerr << errMsgStr << std::endl;
      json res_json;
      res_json["returnCode"] = -1;
      auto mime = Http::Mime::MediaType::fromString("application/json");
      writer.send(Http::Code::Bad_Request, res_json.dump(),mime);
      return;
    }

    //writer.timeoutAfter(std::chrono::milliseconds(60000)); // 1 min limit
    std::vector<double> res_vec;
    std::string serverMsg;
    int serverReturnCode;
    solve(res_vec, serverMsg, serverReturnCode);
    json res_json;
    res_json["res_vec"] = res_vec;
    res_json["msgFromServer"] = serverMsg;
    res_json["returnCode"] = serverReturnCode;
    //json res_json(Vdrop);
    auto mime = Http::Mime::MediaType::fromString("application/json");
    writer.send(Http::Code::Ok, res_json.dump(),mime);
  }

  
  void onTimeout(const Http::Request& req, Http::ResponseWriter response) override {
    UNUSED(req);
    response.send(Http::Code::Request_Timeout, "Timeout").then([=](ssize_t){}, PrintException());  
  }


  // offline test
  void test(std::string const & jsonfilePath)
  {
    std::ifstream ifile(jsonfilePath);
    ifile >> json_obj_;
    std::vector<double> Vdrop;
    std::string serverMsg;
    int serverReturnCode;
    solve(Vdrop, serverMsg, serverReturnCode);
  }
  

  void solve(std::vector<double> & res_vec, std::string & serverMsg,
    int & serverReturnCode, int num_threads = 4) {
    time_t now = time(0);
    char * dt = ctime(&now);
    std::cout<<"-------------solvering IR drop----------------------\n";
    std::cout<<"local date and time: " << dt << std::endl;
    serverMsg = "";
    //omp_set_num_threads(num_threads);
    double start = omp_get_wtime();
    try {
      // need routine to calculate res_vec///////////////
      res_vec = {};
      ////////////////////////////////////////////////
      double end = omp_get_wtime();
      std::cout << "elapsed secs : " << end - start << std::endl; 
      serverReturnCode = 0;
      serverMsg += "calculation successful\n";
    } catch (std::exception & e) { // other exceptions
      std::string errMsgStr(e.what());
      serverMsg +=  errMsgStr;
      serverReturnCode = -1;
      std::cerr << "error : " + errMsgStr << std::endl;
    }
    //mls.outputSol("results.json");  // output field results?
  }

private:
  json json_obj_;
  
};


void checkHostName(int hostname) 
{ 
    if (hostname == -1) 
    { 
        perror("gethostname"); 
        exit(1); 
    } 
} 
  
// Returns host information corresponding to host name 
void checkHostEntry(struct hostent * hostentry) 
{ 
    if (hostentry == NULL) 
    { 
        perror("gethostbyname"); 
        exit(1); 
    } 
} 
  
// Converts space-delimited IPv4 addresses 
// to dotted-decimal format 
void checkIPbuffer(char *IPbuffer) 
{ 
    if (NULL == IPbuffer) 
    { 
        perror("inet_ntoa"); 
        exit(1); 
    } 
} 


void printIPAddress()
{
    char hostbuffer[256]; 
    char *IPbuffer; 
    struct hostent *host_entry; 
    int hostname; 
  
    // To retrieve hostname 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    checkHostName(hostname); 
  
    // To retrieve host information 
    host_entry = gethostbyname(hostbuffer); 
    checkHostEntry(host_entry); 
  
    // To convert an Internet network 
    // address into ASCII string 
    IPbuffer = inet_ntoa(*((struct in_addr*) 
                           host_entry->h_addr_list[0])); 
  
    printf("Hostname: %s\n", hostbuffer); 
    printf("Host IP: %s\n", IPbuffer); 

}



int main()
{
  // test code without sending request from client
  /*std::string jsonfilePath = "../serverinputs.json";
  IRDropHandler irh;
  irh.test(jsonfilePath);*/
  int port = 5000;
  int max_req_size = 20000000; //259 * 1000 * 5;
  printIPAddress();
  std::cout << "port : " << port << std::endl;
  Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(port));
  auto opts = Pistache::Http::Endpoint::options().maxRequestSize(max_req_size);
  Http::Endpoint server(addr);
  server.init(opts);
  server.setHandler(Http::make_handler<TestHandler>());
  server.serve();

}
