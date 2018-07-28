#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#ifdef _MSC_VER
#include <WinSock2.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
std::vector<std::string> getmacs(){
	std::vector<std::string> macs;
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			char mac[32];
			sprintf_s(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
				pIpAdapterInfo->Address[0],
				pIpAdapterInfo->Address[1],
				pIpAdapterInfo->Address[2],
				pIpAdapterInfo->Address[3],
				pIpAdapterInfo->Address[4],
				pIpAdapterInfo->Address[5]);
			macs.push_back(mac);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	return macs;
}
#endif

bool is_mac(std::string& input){
	if (input.size() != 17){
		return false;
	}
	else{
		if (input[2] == ':' && input[5] == ':' && input[8] == ':' && input[11] == ':' && input[14] == ':'){
			input.erase(std::remove(input.begin(),input.end(),':'),input.end());
			if (input.size() != 12){
				return false;
			}
			else{
				for (int i = 0; i < input.size(); ++i){
					if (input[i] >= '0' && input[i] <= '9'){
						input[i] -= 48;
					}
					else{
						if (input[i] >= 'a' && input[i] <= 'f'){
							input[i] -= 87;
						}
						else{
							if (input[i] >= 'A' && input[i] <= 'F'){
								input[i] -= 55;
							}
							else{
								return false;
							}
						}
					}
				}
				return true;
			}
		}
		else{
			return false;
		}
	}
}

int process(int in1, int in2, int in3, char oper1,char oper2){
	++in1;
	++in2;
	++in3;
	int output = 0;
	switch (oper1){
	case '1': output = (in1 + in2)*4; break;
	case '2': output = (in1 - in2)*2; break;
	case '3': output = in1 * in2; break;
	case '4': output = (in1 % in2)*5; break;
	case '5': output = (in1&in2)*10; break;
	case '6': output = (in1 | in2)*12; break;
	case '7': output = (in1^in2)*3; break;
	default:break;
	}
	switch (oper2){
	case '1': output += in3*2; break;
	case '2': output -= in3*12; break;
	case '3': output *= in3*3; break;
	case '4': output %= in3*21; break;
	case '5': output &= in3*2; break;
	case '6': output |= in3*15; break;
	case '7': output ^= in3*9; break;
	default:break;
	}
	return output;
}
int process(int in1, int in2, char oper1){
	++in1;
	++in2;
	int output = 0;
	switch (oper1){
	case '1': output = (in1 + in2)*2; break;
	case '2': output = (in1 - in2)*10; break;
	case '3': output = (in1 * in2)*21; break;
	case '4': output = (in1%in2)*3; break;
	case '5': output = (in1&in2)*9; break;
	case '6': output = (in1|in2)*5; break;
	case '7': output = (in1^in2)*4; break;
	default:break;
	}
	return output;
}

std::string encode_one(std::string input){
	//std::string mask = "343133212334323143413134134323132343133212334323143413134134323132343133212334323143413134134323132";
	std::string mask = "1767136367762177323132416636331351737135346647271734363431332123343231434131341343231324166363313517";
	std::string output;
	int temp = 4543;
	int process_index = 0;
	int value = process(temp, input[0], mask[process_index++]);
	value += (value % 36);
	//std::cout << "value" << value << std::endl;
	output.push_back(char(abs(value%36)));
	for (int i = 1; i < input.size(); ++i){
		value = process(value,output[i-1],input[i],mask[process_index++],mask[process_index++]);
		value += (value % 36);
		//if (i == 1)
		//	std::cout << "value" << value << std::endl;
		output.push_back(char(abs(value%36)));
	}
	return output;
}

std::string encode(std::string input){
	std::reverse(input.begin(), input.end());
	std::string output = encode_one(input);
	output = output + encode_one(output);
	output = encode_one(output)+output;
	for (int i = 0; i < output.size(); ++i){
		if (output[i] >= 0 && output[i] <= 9){
			output[i] += 48;
		}
		else{
			output[i] += 55;
		}
	}
	return output;
}
