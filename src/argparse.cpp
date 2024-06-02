#include <iostream>
#include "argparse.h"
#include <exception>
#include "error.h"

using namespace std;

Arguement::Arguement()
{
}

Arguement::Arguement(const string _name, const string _long_name, string help, int _n_args)
    : name(_name) 
    , long_name(_long_name)
    , present(false)
    , arguments()
    , help_msg(help)
    , n_args(_n_args)
{
}

void
Arguement::set_present(bool _present)
{
    this->present = _present;
}

string
Arguement::get_arg_str()
{
    if (this->n_args < 1)
        critical_error("%s has up to %d arguements", this->long_name.c_str(), this->n_args);
    
    if (!this->present)
        critical_error("%s was not provided on cmldine", this->long_name.c_str());
       
    return this->arguments[0];
}

vector<string>
Arguement::get_args_str()
{
    if (this->n_args < 1)
        critical_error("%s has up to %d arguements", this->long_name.c_str(), this->n_args);
    
    if (!this->present)
        critical_error("%s was not provided on cmldine", this->long_name.c_str());
       
    return this->arguments;
}

int
Arguement::get_arg_int()
{

    if (this->n_args < 1)
        critical_error("%s has up to %d arguements", this->long_name.c_str(), this->n_args);

    if (!this->present)
        critical_error("%s was not provided on cmldine", this->long_name.c_str());
    
 
    return stoi(this->arguments[0]);
}



ArgParser::ArgParser() 
    : arguements()
{
    
}

void
ArgParser::add_arguement(string arg_name, string long_name, int n_args, string help)
{
    shared_ptr<Arguement> arg = shared_ptr<Arguement>(new Arguement(arg_name, long_name, help, n_args));
    this->arguements[arg_name]  = arg;
    this->arguements[long_name] = shared_ptr<Arguement>(arg);
}

void
ArgParser::show_help_dialogue() 
{
    vector<string> args_shown_so_far = vector<string>();

    for (auto i = this->arguements.begin(); i != this->arguements.end(); i++) 
    {
        
        if (std::find(args_shown_so_far.begin(), 
                      args_shown_so_far.end(), 
                      i->second->get_name()) == args_shown_so_far.end()) {
            args_shown_so_far.push_back(i->second->get_name());
            args_shown_so_far.push_back(i->second->get_long_name());

            printf("%s %s    %s\n",
                    i->second->get_name().c_str(),
                    i->second->get_long_name().c_str(),
                    i->second->help().c_str());
        } 
    }

    exit(0);
}

void
ArgParser::parse(int argc, char** argv)
{
    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%s\n", argv[i]);
    // }

    if (this->arguements.size())
    {
        int i = 1;
        for (i = 1; i < argc; i++) 
        {
            //printf("Current Arg: %s\n", argv[i]);
            if (argv[i] == NULL) {
                return;
            }

            if (strcmp(argv[i], "--help") == 0) {
                this->show_help_dialogue();
            }


            // Does container contain argument?
            if (this->arguements.find(argv[i]) != this->arguements.end())
            {
                auto arguement = this->arguements.at(argv[i]);
                arguement->set_present(true);

                // Collect n_args from argv and place into arguement
                if (arguement->n_args)
                {
                    i++;
                    int args_left;
                    for (args_left = arguement->n_args; args_left && argv[i]; args_left--)
                    {
                        if (*argv[i] == '-') {break;}; // Another argument begins

                        arguement->arguments.push_back(argv[i]);
                        if (args_left != 1) i++; // don't increment i if we have all args collected for this param
                    }

                    if (!argv[i] || args_left)
                    {
                        critical_error("Arguement %s expects %d arguements. Only %d was provided", 
                            arguement->long_name.c_str(),
                            arguement->n_args,
                            arguement->n_args - args_left);
                    }
                }

            } else {
                cout << "Unexpected argument \"" << argv[i] << "\" found" << endl;
                exit(-1);
            }
        }
    } else {
        critical_error("No arguements added this arg parser");
    }
    return;
}

shared_ptr<Arguement> &
ArgParser::get_arguement(string arg_name)
{
    return this->arguements.at(arg_name);
}