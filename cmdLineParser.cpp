// sources: https://cplusplus.com/faq/sequences/strings/split/#getline
// https://github.com/pfdinc/Operating_Systems_Mini_Shell/blob/master/shell.cpp

#include <iostream>
#include <cstddef>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <iterator>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

struct redirInputTypes {
    enum rITypes{
        redirInputNone,
        redirInputCreate,
        redirInputAppend};
};

struct redirOutputTypes {
    enum rOTypes{
        redirOutputNone,
        redirOutputCreate,
        redirOutputAppend};
};
typedef struct {
    size_t argc;
    char **argv;
    redirInputTypes::rITypes redirInput;
    redirOutputTypes::rOTypes redirOutput;
} ArgumentStruct;

struct split{
    enum empties_t {empties_ok, no_empties};
};

template <typename Container>
Container& split(
  Container& result,
  const typename Container::value_type& s,
  const typename Container::value_type& delimiters,
  split::empties_t empties = split::empties_ok )
{
  result.clear();
  size_t current;
  size_t next = -1;
  do
  {
    if (empties == split::no_empties)
    {
      next = s.find_first_not_of( delimiters, next + 1 );
      if (next == Container::value_type::npos) break;
      next -= 1;
    }
    current = next + 1;
    next = s.find_first_of( delimiters, current );
    result.push_back( s.substr( current, next - current ) );
  }
  while (next != Container::value_type::npos);
  return result;
}



ArgumentStruct &readCommand(string& s){
    ArgumentStruct *tmps = new ArgumentStruct;
    tmps->redirInput = redirInputTypes::redirInputNone;
    tmps->redirOutput = redirOutputTypes::redirOutputNone;
    vector<string> progArgs;

    size_t found = s.find(">>");
    if (found != string::npos){
        s.replace(found, 2, ">>", 4);
        tmps->redirOutput = redirOutputTypes::redirOutputAppend;
    }
    else{
        found = s.find(">");
        if (found != string::npos){
            s.replace(found, 1, ">", 3);
            tmps->redirOutput = redirOutputTypes::redirOutputCreate;
        }
    }
    found = s.find("<<");
    if (found != string::npos){
        s.replace(found, 2, "<<", 4);
        tmps->redirInput = redirInputTypes::redirInputAppend;

    }
    else{
        found = s.find("<");
        if (found != string::npos){
            s.replace(found, 1, "<", 3);
            tmps->redirInput = redirInputTypes::redirInputCreate;
        }
    }
    split (progArgs, s, "\n", split::no_empties);

    tmps->argc = progArgs.size();
    
    char **workingStorage = new char*[progArgs.size() + 1];
    for (size_t i = 0; i < progArgs.size(); ++i){
        workingStorage[i] = new char[progArgs[i].size() + 1];
        strcpy(workingStorage[i], progArgs[i].c_str());
    }

    workingStorage[progArgs.size()] = (char *)NULL;
    tmps->argv = workingStorage;
    return *tmps;
}

void  myExecvp(char **argv)
{
    pid_t  pid;
    int status;
    int childStatus;
    pid = fork();
    if(pid == 0)
    {
        childStatus = execvp(*argv, argv);
        if (childStatus < 0){
            std::cout<<"Child pid = -1"<<std::endl;
        }
        exit(0);

    }
    else if(pid < 0)
    {
        std::cout<< "somthing went wrong!"<<std::endl;
    }
    else

    {   int status;
        waitpid(pid, &status , 0);

    }
}

void GetEnv()
{
    char *path2;
    char *arr2[250];
    char *Tokenized ;
    path2 = getenv("PATH");
    Tokenized = strtok(path2, ":");
    int k = 0;
    while(Tokenized != NULL)
    {
        arr2[k] = Tokenized;
        Tokenized = strtok(NULL, ":");
        k++;
    }

    arr2[k] = NULL;
}




int main(int argc, const char * argv[]) {
    string input;
    ArgumentStruct as;
    cout << "Welcome to Mini Shell! \n\n";
    string command(" ");
    
    while(command.compare("exit"))
    {
    cout << "miniShell (^ + C to exit) -> " << flush;
    const int length = input.length();
        char* char_array = new char[length + 1];
        strcpy(char_array, input.c_str());
    getline(cin, input, '\n');
    
    as = readCommand(input);

        if (strcmp(char_array, "exit") == 0)

        {
            break;
        }
        else if (strcmp(char_array, "\n") == 0){
            continue;
        }
        myExecvp(as.argv);
        cout << "Command argument count: " << as.argc << endl;
        for (int i = 0; i < as.argc; i++){
            cout << "Token [" << i << "] : " << as.argv[i] << endl;
        }
        if (as.redirInput != redirInputTypes::redirInputNone){
            cout << "Redirection (input) command detected" << endl;
        }
        if (as.redirOutput != redirOutputTypes::redirOutputNone){
            cout << "Redirection (output) command detected" << endl;
        }
    }
    return 0;
}
