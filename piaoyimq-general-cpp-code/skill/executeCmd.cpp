/*
 * executeCmd.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: azhweib
 */




int executeCommand(const string& sCommand, vector<string>& vReturn )
{

    string sLine;
    FILE *fMyPipe;
    char caBuffer[1024];

    vReturn.clear();

    string sPipeCommand = sCommand + " 2<&1";

    fMyPipe = popen(sCommand.c_str(),"r");

    if ( fMyPipe == NULL ) {
        return -1;
    }

    while (!feof(fMyPipe)) {
        char *s = fgets(caBuffer,1024,fMyPipe);
        if (s==NULL) {
            int iErrorCode = pclose(fMyPipe);
            return iErrorCode;
        }
        vReturn.push_back(caBuffer);
    }

    int iErrorCode = pclose(fMyPipe);

    //remove last element in the list. extra empty line not needed.
    vReturn.pop_back();
    return iErrorCode;
}
