// Outreachy: ceph - radosgw-admin application
// Boost::program_options demo

#include <boost/program_options.hpp>
#include <boost/version.hpp>
#include <iostream>

void print_cmds(){
  std::cout << "commands:\n"
    << "  user create   \tcreate a new user \n"
    << "  user delete   \tdelete a user \n"
    << "  user info     \tget user info \n"
    << std::endl;
}

int main( int argc, char** argv )
{
    namespace po = boost::program_options;

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

    po::variables_map vm_opt;
    po::variables_map vm_all;

    try {

        // handle options (--)
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).run();
        po::store(parsed, vm_opt);
        po::notify(vm_opt);

        if (vm_opt.count("help")) {
          std::cout <<  "usage: radosgw-admin <cmd> [options...]" << std::endl;
          print_cmds();
          std::cout << desc;
          return 0;
        }

        if(vm_opt.count("uid")) {
          user_id = vm_opt["uid"].as<int>();
          std::cout << "uid: " << user_id << std::endl;
        }

        if(vm_opt.count("email")) {
          std::cout << "email: " << email << std::endl;
        }

        if(vm_opt.count("display-name")) {
          std::cout << "display name: " << display_name << std::endl;
        }

        // handle commands
        // method 1: use args and handle the supported commands like the original application (not doing this)
        // method 2: trying to make program_options help us out... it is not as nice as it could be
        po::parsed_options parsed_all = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
        po::store(parsed_all, vm_all);

        std::string cmd_output;
        std::vector<std::string> commands = collect_unrecognized(parsed_all.options, po::include_positional);
        for(std::vector<std::string>::const_iterator cmd = commands.begin(); cmd != commands.end(); ++cmd){

          if(*cmd == "user") {
            cmd++;
            if(cmd == commands.end())
              throw po::required_option("user [create delete info]");

            if(*cmd == "create"){
              std::cout << "user created with uid " << user_id << ", display name " 
                << display_name << " and email " << email << std::endl; 
            }
            if(*cmd == "delete"){
              std::cout << "user deleted with uid " << user_id << std::endl;
            }
            if(*cmd == "info"){
              std::cout << "user info for uid " << user_id << std::endl;
            }
          } else {
            // because unregistered options were allowed, throw manually if 
            // there is a command we don't handle
            throw po::unknown_option(*cmd);
          }
        }

    } catch ( const boost::program_options::error& e ) {
      std::cerr << e.what() << std::endl;
      std::cout <<  "usage: radosgw-admin <cmd> [options...]" << std::endl;
      print_cmds();
      std::cout << desc << std::endl;
    }
}