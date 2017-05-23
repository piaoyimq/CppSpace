#include <iostream>
#include <stdlib.h>



int runSystemCommand(const std::string& command)
{

    int result = -1;
    if (std::system(NULL))
    {
        result = std::system(command.c_str()); // execute the command
    }
    else
    {
        // some system environment error, not expected to end up here
        std::cout << "no command handler available" << std::endl;
    }

    return WEXITSTATUS(result);
}


int main()
{
	int a = runSystemCommand("ls");
	if(a> 0)
	{
		std::cout << "Execute the command successuflly" << std::endll;
	}
}
