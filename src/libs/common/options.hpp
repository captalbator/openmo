#pragma once

class OptionsParser
{
public:
  explicit OptionsParser(std::string description);

  typedef std::variant<int32_t *, uint32_t *, double *, float *, bool *, std::string *> Value;

  void addArgument(std::vector<std::string> const &flags, Value const &defaultValue,
                   std::string const &description);

  void parse(int argc, char **argv) const;
  void printHelp(std::ostream &os = std::cout) const;

private:
  struct Argument
  {
    std::vector<std::string> flags;
    Value value;
    std::string description;
  };

  std::string _description;
  std::vector<Argument> _arguments;
};
