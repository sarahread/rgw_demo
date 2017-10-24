// Outreachy: ceph - radosgw-admin application
// Boost::program_options demo

#include <boost/program_options.hpp>
#include <iostream>
#include "commands.h"

namespace po = boost::program_options;

int main( int argc, char** argv )
{
  po::options_description desc("options");

  int user_id;
  std::string email;
  std::string display_name;

  desc.add_options()
    ("help,h",                                   "display help")
    ("uid",           po::value<int>(),          "user id") 
    ("email",         po::value(&email),         "email")
    ("display-name",  po::value(&display_name),  "display name")
  ;

  const Command* operation = NULL;
  std::string current;
  std::string previous;

  po::variables_map vm;

  try {

    // Use argv directly to obtain the commands (usually 2 commands, can be 1)
    int argv_index = 1;
    while(argv_index < argc && !operation){
      // Leave "--" options for boost
      if(std::string(argv[argv_index]).find("--") == 0){
        //argv_index++;
        //continue;
        break;
      }
      else { // First command string (Command.cmd)
        if(previous.empty()){
          for(const auto& command : commands) {
            if(argv[argv_index] == command.cmd){
              previous = std::string(argv[argv_index]);
              if(command.action.empty())
                operation = &command;
            }
          }
          if(previous.empty()){
            throw po::unknown_option(argv[argv_index]);
          }
        }
        else { // Second command string (Command.action)
          for(const auto& command : commands) {
            if(previous == command.cmd && argv[argv_index] == command.action){
              operation = &command;
            }
          }
          if(!operation){
            throw po::unknown_option(argv[argv_index]);
          }
        }
      }
      argv_index++;
    }

    // Error: second command string missing
    if(!previous.empty() && !operation) {
      std::vector<std::string> alternatives;
      for(const auto& command : commands) {
        if(previous == command.cmd){
          alternatives.push_back(command.action);
        }
      }
      throw po::ambiguous_option(alternatives);
    }

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "usage: radosgw-admin <cmd> [options...]" << std::endl;
        for(const auto& it : commands) {
          std::cout << it;
        }
        std::cout << desc;
    }

    // Option Handlers
    if(vm.count("uid")) {
      user_id = vm["uid"].as<int>();
      std::cout << "uid: " << user_id << std::endl;
    }

    if(vm.count("email")) {
      std::cout << "email: " << email << std::endl;
    }

    if(vm.count("display-name")) {
      std::cout << "display name: " << display_name << std::endl;
    }

    // Command Handlers
    // TODO: verify that the correct options were passed
    // possible to eliminate switch? function pointers?
    if(operation) {

      switch(operation->id){

        case OPT_USER_CREATE:
          std::cout << "user created with uid " << user_id << ", display name " 
          << display_name << " and email " << email << std::endl;
            break;

        case OPT_USER_INFO: 
          std::cout << "user info for uid " << user_id << std::endl;
          break;

        case OPT_USER_DELETE:
          std::cout << "user deleted with uid " << user_id << std::endl;
          break;

        default:
          break;
      }

      std::cout << operation->cmd
        << (operation->action.empty() ? "" : " " + operation->action )
        << " complete." << std::endl;
    }

  } catch ( const boost::program_options::error& e ) {
    std::cerr << e.what() << std::endl;
    std::cout <<  "usage: radosgw-admin <cmd> [options...]" << std::endl;
    
    for(const auto& it : commands) {
      std::cout << it;
    }
    std::cout << desc << std::endl;
  }
}