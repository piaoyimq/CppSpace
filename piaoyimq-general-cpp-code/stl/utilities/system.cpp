#include <iostream>
#include <stdlib.h>



int runSystemCommand(const std::string& command)
{

    int result = -1;
    if (system(NULL))
    {
        result = system(command.c_str()); // execute the command
    }
    else
    {
        // some system environment error, not expected to end up here
        std::cout << "no command handler available" << std::endl;
    }

    return WEXITSTATUS(result);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

bool runSystemCommand2(const char* cmd)
{
    pid_t status;


    status = system(cmd);

    if (-1 == status)
    {
        printf("system error!");
        return false;
    }
    else
    {
        printf("exit status value = [0x%x]\n", status);

        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                printf("run shell script successfully.\n");
                return true;
            }
            else
            {
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
                return false;
            }
        }
        else
        {
            printf("exit status = [%d]\n", WEXITSTATUS(status));
            return false;
        }
    }

    return 0;
}

int main()
{
	int a = runSystemCommand("lss");
	if(a> 0)
	{
		std::cout << "Execute the command successuflly" << std::endl;
	}

	bool ret = runSystemCommand2("lss");
	if(!ret)
	{
	    std::cout << "Execute the command failed" << std::endl;
	}

	ret = ::system("lss");
	if(ret)
	{
	    std::cout << "Execute the command failed" << std::endl;
	}
}
