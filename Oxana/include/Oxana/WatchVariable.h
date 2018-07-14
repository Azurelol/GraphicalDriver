#pragma once
#include "GraphicalDriverInclude.h"

namespace Oxana
{
  struct WatchVariableBase
  {
    std::string name;
    std::string lastValueToString;
    virtual void Record() = 0;
  };

  struct PrintVariableValue : WatchVariableBase
  {
    LogFunction printFunction;

    PrintVariableValue(const std::string& name, LogFunction printFunction)
    {
      this->name = name;
      this->printFunction = printFunction;
    }

    void Record()
    {
      lastValueToString = printFunction();
    }


  };

  template <typename ValueType>
  struct WatchVariable : WatchVariableBase
  {
    ValueType* variable;
    bool isPointer;


    WatchVariable(const std::string& name, ValueType* variable)
      : variable(variable)
    {
      this->name = name;
      // not working
      isPointer = std::is_pointer<ValueType>::value;
    }
    
    void Record()
    {
      std::stringstream ss;
      if (!isPointer)
        ss << std::boolalpha << *variable;
      else 
        ss << std::boolalpha << variable;
      lastValueToString = ss.str();
    }

  };

  struct PlotVariableBase : WatchVariableBase
  {
    std::vector<float> history;
    unsigned bufferSize;
  };

  // A type that can be cast to a float!
  template <typename NumericType>
  struct PlotVariable : PlotVariableBase
  {
    NumericType* variable;

    PlotVariable(const std::string& name, NumericType* variable)
      : variable(variable)
    {
      this->name = name;
    }

    // Rercods the the current value of the variable
    void Record()
    {
      // Update the history
      if (history.size() > bufferSize)
      {
        history.erase(history.begin());
      }
      history.push_back(*variable);

      // Record the latest variable
      std::stringstream ss;
      ss << *variable;
      lastValueToString = ss.str();

    }
    
  };
}