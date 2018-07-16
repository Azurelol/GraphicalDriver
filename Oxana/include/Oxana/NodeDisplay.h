#pragma once
#include "Include.h"

namespace Oxana
{ 

  struct NodeDisplay
  {
    using Description = std::string;
    struct Data
    {
      Description nodeData;
      Description parentData;
      std::vector<Description> childrenData;

			void Clear() {
				nodeData = Description();
				parentData = Description();
			  childrenData.clear();
			}
    };

    // The latest node data
    Data latest;
    virtual void Record() = 0;
		void Clear() 
		{
			latest.Clear();
		}
  };

  // Contains relevant information for how to display a node
  template <typename NodeType>
  struct GenericNodeDisplay : NodeDisplay
  {
    // Aliases
    using GetNodeFunction = std::function<typename NodeType*(void)>;
    using DescriptionFunction = std::function<std::string(typename NodeType*)>;
    using GetParentFunction = std::function<typename NodeType*(typename NodeType*)>;
    using GetChildrenFunction = std::function < std::vector<typename NodeType*>(typename NodeType*)>;

    // Gets the node that is to be inspected
    GetNodeFunction getNode;
    // A function that will be used to describe a given node
    DescriptionFunction getDescription;
    // A function to get a node's parent
    GetParentFunction getParent;
    // A function to get a node's children
    GetChildrenFunction getChildren;

    void Record()
    {
      latest = Data();

      // Node
      NodeType* node = getNode();
      latest.nodeData = getDescription(node);
      // Parent
      NodeType* parent = getParent(node);
      if (parent != nullptr)
        latest.parentData = getDescription(parent);
      // Children
      std::vector<typename NodeType*> children = getChildren(node);
      for (auto& child : children)
        latest.childrenData.push_back(getDescription(child));


    }

  };

}