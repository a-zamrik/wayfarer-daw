#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class Arguement
{
      friend class ArgParser;

protected:
    std::string              name;
    std::string              long_name;
    bool                     present;
    std::string              help_msg;
    int                      n_args;

private:
    std::vector<std::string> arguments;

    void __check_if_present();


public:
    Arguement();
    Arguement(const std::string, const std::string, const std::string, int);

    void set_present(const bool);
    inline bool is_present() const {return this->present;}
    inline std::string help() const {return this->help_msg;}
    inline std::string get_name() const {return this->name;}
    inline std::string get_long_name() const {return this->long_name;}
    
    std::string get_arg_str();
    std::vector<std::string> get_args_str();
    int get_arg_int();
};



class ArgParser
{
    std::unordered_map<std::string, std::shared_ptr<Arguement>> arguements;

private:
  void show_help_dialogue();

public:

  ArgParser();

  void add_arguement(std::string, std::string, int, std::string help = "");

  void parse(int argc, char** argv);

  std::shared_ptr<Arguement> & get_arguement(std::string);

};

#endif
