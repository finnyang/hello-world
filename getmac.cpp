#include <iostream>
#include <vector>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <net/if.h>

std::vector<std::string> getnames(){
  std::vector<std::string> names;
  struct sockaddr_in *sin = NULL;
  struct ifaddrs *ifa = NULL, *ifList;
  if (getifaddrs(&ifList) < 0);
  int i = 0;
  for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next){
      if(ifa->ifa_addr->sa_family == AF_INET){
        names.push_back(ifa->ifa_name);
      }
  }
  return names;
}

std::vector<std::string> getmacs(std::vector<std::string> names){
  std::vector<std::string> macs;
  char uc_Mac[100];
  int fd;
  char *mac;
  struct ifreq ifr;
  for(int i = 0; i < names.size(); ++i){
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy((char *)ifr.ifr_name , names[i].c_str() , IFNAMSIZ-1);
    ioctl(fd, SIOCGIFHWADDR, &ifr);
    close(fd);
    mac = (char *)ifr.ifr_hwaddr.sa_data;
    sprintf((char *)uc_Mac,(const char *)"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x" , mac[0]&0xff, mac[1]&0xff, mac[2]&0xff, mac[3]&0xff, mac[4]&0xff, mac[5]&0xff);
    macs.push_back(uc_Mac);
  }
  return macs;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> names= getnames();
    std::vector<std::string> macs=getmacs(names);
    for(int i = 0; i < names.size();++i){
      std::cout << names[i] << "  " << macs[i] << std::endl;
    }
    return 0;
}
