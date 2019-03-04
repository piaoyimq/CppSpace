#include <mcheck.h>
#include <string>
#include <iostream>
#include "../CJsonObject.hpp"

void test1()
{
    mtrace();
    int iValue;
    double fTimeout;
    std::string strValue;
    neb::CJsonObject oJson("{\"refresh_interval\":60,"
                        "\"timeout\":12.5,"
                        "\"dynamic_loading\":["
                            "{"
                                "\"so_path\":\"plugins/User.so\", \"load\":false, \"version\":1,"
                                "\"cmd\":["
                                     "{\"cmd\":2001, \"class\":\"neb::CmdUserLogin\"},"
                                     "{\"cmd\":2003, \"class\":\"neb::CmdUserLogout\"}"
                                "],"
                                "\"module\":["
                                     "{\"path\":\"im/user/login\", \"class\":\"neb::ModuleLogin\"},"
                                     "{\"path\":\"im/user/logout\", \"class\":\"neb::ModuleLogout\"}"
                                "]"
                             "},"
                             "{"
                             "\"so_path\":\"plugins/ChatMsg.so\", \"load\":false, \"version\":1,"
                                 "\"cmd\":["
                                      "{\"cmd\":2001, \"class\":\"neb::CmdChat\"}"
                                 "],"
                             "\"module\":[]"
                             "}"
                        "]"
                    "}");
     std::cout << oJson.ToString() << std::endl;
     std::cout << "-------------------------------------------------------------------" << std::endl;
     std::cout << oJson["dynamic_loading"][0]["cmd"][1]("class") << std::endl;
     oJson["dynamic_loading"][0]["cmd"][0].Get("cmd", iValue);
     std::cout << "iValue = " << iValue << std::endl;
     oJson["dynamic_loading"][0]["cmd"][0].Replace("cmd", -2001);
     oJson["dynamic_loading"][0]["cmd"][0].Get("cmd", iValue);
     std::cout << "iValue = " << iValue << std::endl;
     oJson.Get("timeout", fTimeout);
     std::cout << "fTimeout = " << fTimeout << std::endl;
     oJson["dynamic_loading"][0]["module"][0].Get("path", strValue);
     std::cout << "strValue = " << strValue << std::endl;
     std::cout << "-------------------------------------------------------------------" << std::endl;
     oJson.AddEmptySubObject("depend");
     oJson["depend"].Add("nebula", "https://github.com/Bwar/Nebula");
     oJson["depend"].AddEmptySubArray("bootstrap");
     oJson["depend"]["bootstrap"].Add("BEACON");
     oJson["depend"]["bootstrap"].Add("LOGIC");
     oJson["depend"]["bootstrap"].Add("LOGGER");
     oJson["depend"]["bootstrap"].Add("INTERFACE");
     oJson["depend"]["bootstrap"].Add("ACCESS");
     std::cout << oJson.ToString() << std::endl;
     std::cout << "-------------------------------------------------------------------" << std::endl;
     std::cout << oJson.ToFormattedString() << std::endl;

     std::cout << "-------------------------------------------------------------------" << std::endl;
     neb::CJsonObject oCopyJson = oJson;
     if (oCopyJson == oJson)
     {
         std::cout << "json equal" << std::endl;
     }
     oCopyJson["depend"]["bootstrap"].Delete(1);
     oCopyJson["depend"].Replace("nebula", "https://github.com/Bwar/CJsonObject");
     std::cout << oCopyJson.ToString() << std::endl;
     std::cout << "-------------------------key traverse------------------------------" << std::endl;
     std::string strTraversing;
     while(oJson["dynamic_loading"][0].GetKey(strTraversing))
     {
         std::cout << strTraversing << std::endl;
     }
     std::cout << "---------------add a new key, then key traverse---------------------" << std::endl;
     oJson["dynamic_loading"][0].Add("new_key", "new_value");
     while(oJson["dynamic_loading"][0].GetKey(strTraversing))
     {
         std::cout << strTraversing << std::endl;
     }
}


void test2()
{
    std::string jsonString("{\"sys\": [{\"v\": \"1\", \"id\": \"k1\", \"key\": \"receiving SRQ from prober timed out\"}, {\"v\": \"1\", \"id\": \"k2\", \"key\": \"waiting for operation to complete, heartbeat timeout\"}, {\"v\": \"1\", \"id\": \"k3\", \"key\": \"waiting_for_lot_timeout\"}, {\"v\": \"0\", \"id\": \"k4\", \"key\": \"general_timeout\"}, {\"v\": \"0\", \"id\": \"k5\", \"key\": \"ERROR\"}, {\"v\": \"0\", \"id\": \"k6\", \"key\": \"Error\"}, {\"v\": \"0\", \"id\": \"k7\", \"key\": \"error\"}, {\"v\": \"0\", \"id\": \"k8\", \"key\": \"Not Calibrated\"}], \"user\": [{\"v\": \"1\", \"key\": \"teseterddd\"}]}");
    neb::CJsonObject json(jsonString);

    std::cout << json.ToString() << std::endl;


    char keys[2][20]={"sys", "user"};
    for(unsigned int j=0; j<sizeof(keys)/sizeof(keys[0]);j++)
    {
        for(int i=0; i <json[keys[j]].GetArraySize(); i++)
        {
            std::cout << json[keys[j]][i].ToString() << std::endl;
            std::string v;
            std::string key;
            if(!json[keys[j]][i].Get("v", v))
            {
                continue;
            }

            if("1"==v)
            {
                if(json[keys[j]][i].Get("key", key))
                {
                    std::cout << key << std::endl;
                }

            }
        }
    }
}


void test3()
{
    std::string jsonString("[{\"event\": \"ACTION_ASSIGNMENT_EVENT\", \"vname\": \"QQQ\"}, {\"event\": \"ACTION_ASSIGNMENT_EVENT\", \"vname\": \"FAMILY2\"}, {\"event\": \"LOT_STARTED\", \"vname\": \"STAGE\"}]");
    neb::CJsonObject json(jsonString);

    std::cout << json.ToString() << std::endl;
    for(int i=0; i <json.GetArraySize(); i++)
    {
        std::cout << json[i].ToString() << std::endl;
        std::string event;
        std::string variable;
        if(!json[i].Get("event", event) || !json[i].Get("vname", variable))
        {
            continue;
        }
        std::cout << event << ":" << variable << std::endl;
    }
}

int main()
{
//    test1();
//    test2();
    test3();
}
