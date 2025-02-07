#include "options.hpp"

// https://schneegans.github.io/tutorials/2019/08/06/commandline
OptionsParser::OptionsParser(std::string description) : _description(std::move(description))
{
}

void OptionsParser::addArgument(std::vector<std::string> const &flags, Value const &defaultValue,
                                std::string const &description)
{
  _arguments.emplace_back(Argument{flags, defaultValue, description});
}

void OptionsParser::printHelp(std::ostream &os) const
{
  os << _description << std::endl;

  uint32_t maxFlagLength = 0;
  for (auto const &argument : _arguments)
  {
    uint32_t flagLength = 0;
    for (auto const &flag : argument.flags)
    {
      flagLength += static_cast<uint32_t>(flag.size()) + 2;
    }

    maxFlagLength = std::max(maxFlagLength, flagLength);
  }

  for (auto const &argument : _arguments)
  {
    std::string flags;
    for (auto const &flag : argument.flags)
    {
      flags += flag + ", ";
    }

    // Add padding according to longest flag (so everything lines up nicely)
    std::stringstream sstr;
    sstr << std::left << std::setw(maxFlagLength) << flags.substr(0, flags.size() - 2);

    size_t spacePos = 0;
    size_t lineWidth = 0;
    while (spacePos != std::string::npos)
    {
      size_t nextSpacePos = argument.description.find_first_of(' ', spacePos + 1);
      sstr << argument.description.substr(spacePos, nextSpacePos - spacePos);
      lineWidth += nextSpacePos - spacePos;
      spacePos = nextSpacePos;

      if (lineWidth > 60)
      {
        os << sstr.str() << std::endl;
        sstr = std::stringstream();
        sstr << std::left << std::setw(maxFlagLength - 1) << " ";
        lineWidth = 0;
      }
    }
  }
}

void OptionsParser::parse(int argc, char **argv) const
{
  int i = 1;
  while (i < argc)
  {
    std::string flag(argv[i]);
    std::string value;
    bool isValueSeparate = false;

    size_t equalPos = flag.find('=');
    if (equalPos != std::string::npos)
    {
      value = flag.substr(equalPos + 1);
      flag = flag.substr(0, equalPos);
    }
    else if (i + 1 < argc)
    {
      value = argv[i + 1];
      isValueSeparate = true;
    }

    bool foundArg = false;
    for (auto const &argument : _arguments)
    {
      if (std::find(argument.flags.begin(), argument.flags.end(), flag) != std::end(argument.flags))
      {
        foundArg = true;

        // If the argument is a boolean and the value is empty (or not a boolean) then the
        // flag should be true
        if (std::holds_alternative<bool *>(argument.value))
        {
          // If the value isn't true or false then it means it is another argument
          if (!value.empty() && value != "true" && value != "false")
          {
            isValueSeparate = false;
          }
          *std::get<bool *>(argument.value) = (value != "false");
        }
        // If the value isn't a boolean but is empty then we throw an error
        else if (value.empty())
        {
          throw std::runtime_error("Failed to parse command line arguments: "
                                   "Missing value for argument \"" +
                                   flag + "\"!");
        }
        // If the value is a string then we can directly set it
        else if (std::holds_alternative<std::string *>(argument.value))
        {
          *std::get<std::string *>(argument.value) = value;
        }
        else
        {
          std::visit(
              [&value](auto &&arg)
              {
                std::stringstream sstr(value);
                sstr >> *arg;
              },
              argument.value);
        }

        break;
      }
    }

    // Unknown argument
    if (!foundArg)
    {
      std::cerr << "unknown command line argument \"" << flag << "\"" << std::endl;
    }

    ++i;

    // Advance again if the next entry is a value
    if (foundArg && isValueSeparate)
    {
      ++i;
    }
  }
}
