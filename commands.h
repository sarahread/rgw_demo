// Outreachy: ceph - radosgw-admin application
// Boost::program_options demo
// Commands.h

struct Command 
{
  int id;
  std::string cmd;
  std::string action;
  std::string text;
};

std::ostream& operator<<(std::ostream& os, const Command& c) {
  return os << "  " << c.cmd << " " << c.action << "\t\t" << c.text << std::endl;
}

enum {
  OPT_NO_CMD = 0,
  OPT_USER_CREATE,
  OPT_USER_INFO,
  OPT_USER_DELETE,
  OPT_USER_SUSPEND,
  OPT_USER_ENABLE,
  OPT_USER_CHECK,
  OPT_USER_STATS,
  OPT_USER_LIST,
  OPT_POLICY
};

std::vector<Command> commands = {
  { OPT_USER_CREATE,    "user",     "create",   "create a new user"},
  { OPT_USER_INFO,      "user",     "info",     "get user info"},
  { OPT_USER_DELETE,    "user",     "delete",   "remove user"},
  { OPT_USER_SUSPEND,   "user",     "suspend",  "suspend a user"},
  { OPT_USER_ENABLE,    "user",     "enable",   "re-enable user after suspension"},
  { OPT_USER_CHECK,     "user",     "check",    "check user info"},
  { OPT_USER_STATS,     "user",     "stats",    "show user stats as accounted by quota subsystem"},
  { OPT_USER_LIST,      "user",     "list",     "list users"},
  { OPT_POLICY,         "policy",   "",         "read bucket/object policy"}
};