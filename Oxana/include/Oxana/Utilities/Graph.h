#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <type_traits>
#include <cstddef>

namespace Oxana
{
	// A simple class for creating generic graphs
	class Graph
	{
		public:

		//--------------------------------------------------------------------------/
		// Declarations
		//--------------------------------------------------------------------------/
		struct Node;
		struct Edge;
		struct AdjacencyMatrix;
		struct AdjacencyList;

		//--------------------------------------------------------------------------/
		// Aliases
		//--------------------------------------------------------------------------/
		// A shared reference to a node
		using NodeReference = std::shared_ptr<Node>;

		// A list of nodes
		using NodeList = std::vector<NodeReference>;

		// Returns the neighbors of a given node
		using NeighborFunction = std::function<NodeList(const NodeReference&)>;

		// Determines whether two nodes are connected together
		using ConnectedFunction = std::function<bool(NodeReference)>;

		// Returns the cost it takes to travel between nodes, with a negative value being not traversible
		using CostFunction = std::function<float(const NodeReference&, const NodeReference&)>;

		// Calculates the heuristc cost of a node
		using HeuristicFunction = CostFunction;

		//--------------------------------------------------------------------------/
		// Definitions
		//--------------------------------------------------------------------------/

		/////////////////////////////////////////////////////////////////////////////
		// Base class for nodes used by the graph
		/////////////////////////////////////////////////////////////////////////////
		struct Node
		{
			enum class Status { Open, Closed, Unexplored };

			//--------------------------------------------------------------------------/
			// Fields
			//--------------------------------------------------------------------------/
			// Unique identifier for this node
			unsigned id;
			// Whether this node is on the open or closed list of a search
			Status status;
			// Data associated to this node
			//std::shared_ptr<void> data;			
			void* data;			
			// The node which led to this one from the starting node
			NodeReference parent;
			// f(x) = g(x) + h(x) 
			// cost = given cost + heuristic cost
			float cost;
			// g(x): How much it costs to get back to the starting node
			float givenCost;
			// The neighbors to this node
			NodeList neighbors;

			//--------------------------------------------------------------------------/
			// CTOR
			//--------------------------------------------------------------------------/
			Node() : parent(nullptr), cost(0), givenCost(0), status(Status::Unexplored)
			{
			}

			Node(NodeReference parent) : parent(parent), cost(0), givenCost(0), status(Status::Unexplored)
			{
			}

			//--------------------------------------------------------------------------/
			// Methods
			//--------------------------------------------------------------------------/
			template <typename T>
			T* GetData()
			{
				return static_cast<T*>(data);
			}

			template <typename T>
			void SetData(T* data)
			{
				//this->data = std::make_shared<T>(data);
				this->data = static_cast<void*>(data);
			}

			bool Traversible() { return cost > 0; }
		};

		/////////////////////////////////////////////////////////////////////////////
		// Base class for nodes used by the graph
		/////////////////////////////////////////////////////////////////////////////
		struct AdjacencyMatrix
		{

		};

		/////////////////////////////////////////////////////////////////////////////
		// Base class for nodes used by the graph
		/////////////////////////////////////////////////////////////////////////////
		struct Edge
		{

		};

		/////////////////////////////////////////////////////////////////////////////////
		// Adjacency list is a vector that for each vertex, has a vector of its neighbors
		/////////////////////////////////////////////////////////////////////////////////
		struct AdjacencyList
		{

		};


		/////////////////////////////////////////////////////////////////////////////
		// Base class for nodes used by the graph
		/////////////////////////////////////////////////////////////////////////////

		//--------------------------------------------------------------------------/
		// Static Methods
		//--------------------------------------------------------------------------/
		static NodeReference MakeNode()
		{
			return std::make_shared<Node>();
		}

		template <typename T>
		static NodeReference MakeNode(T data)
		{
			NodeReference node = std::make_shared<Node>();
			node->SetData(data);
			return node;
		}

		template <typename T>
		static NodeList Search(T* source,
			                     std::function<float(T*, T*)> costFunction,
													 std::function<std::vector<T*>(const Graph::NodeReference&)> neighborFunction)
		{
			// We will be creating a node for every value
			NodeList nodes;
			unsigned createdNodes = 0;
			// Used to keep track of all created nodes
			std::unordered_map<T*, NodeReference> visited;
			// The open list which will be used for the search
			NodeList openList;

			// Used when making nodes
			static auto makeNode = [&](T* value) -> NodeReference
			{
				NodeReference newNode = MakeNode(value);
				newNode->id = createdNodes++;
				visited.insert(std::make_pair(newNode->GetData<T>(), newNode));
				nodes.push_back(newNode);
				return newNode;
			};

			// Add the source node to the open list
			NodeReference sourceNode = makeNode(source);
			sourceNode->cost = 0;
			openList.push_back(sourceNode);

			// While the open list is not empty
			while (!openList.empty())
			{
				NodeReference parent = PopCheapestNode(openList);
				std::vector<T*> neighbors = neighborFunction(parent);

				for (auto& neighbor : neighbors)
				{
					// Get the node for this value
					NodeReference node;
					bool isVisited = visited.count(neighbor) > 0;
					if (isVisited)
					{
						node = visited[neighbor];
					}
					else
					{
						node = makeNode(neighbor);
					}

					// Get the cost from traveling to this node from the parent
					float traversalCost = costFunction(parent->GetData<T>(), node->GetData<T>());
					bool canTraverse = traversalCost > 0;
					if (!canTraverse)
					{
						node->cost = -1;
						node->status = Node::Status::Closed;
						continue;
					}

					// If the node isn't explored add it to the open list
					bool isExplored = node->status != Node::Status::Unexplored;
					if (!isExplored)
					{						
						node->parent = parent;
						node->givenCost = parent->givenCost + traversalCost;
						node->cost = node->givenCost;
						node->status = Node::Status::Open;
						openList.push_back(node);
					}
					// If the node is on either list
					else
					{
						float newGivenCost = node->givenCost + traversalCost;
						float newCost = newGivenCost;
						if (newCost < node->cost)
						{
							node->parent = parent;
							node->cost = newCost;
							node->status = Node::Status::Open;
							openList.push_back(node);
						}
					}				
				}

				// Place parent on the closed list
				parent->status = Node::Status::Closed;
			}


			// Sort the list, according to shortest distances
			//NodeList nodes;
			return nodes;
		}

		private:
		static NodeReference PopCheapestNode(NodeList& list)
		{
			int cheapestIndex = 0;
			for (int i = 0; i < list.size(); i++)
			{
				auto& node = list[i];
				if (node->cost < list[cheapestIndex]->cost)
				{
					cheapestIndex = i;
				}
			}

			NodeReference cheapest = list[cheapestIndex];
			std::swap(list[cheapestIndex], list.back());
			list.pop_back();
			return cheapest;
		}




	};

}