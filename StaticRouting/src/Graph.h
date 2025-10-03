#pragma once

#include <memory>

#include "Node.h"

class Graph
{
public:
	Graph(): m_size(0) {}

	Node* add_node(std::string& label);
	Node* add_node(Node& node);
	Node* get_node(std::string& label);
	void rename_node(std::string& old_label, std::string& new_label);
	std::vector<Node*> nodes();
	void remove_node(std::string& label);
	void add_edge(std::string& label1, std::string& ip1, std::string& label2, std::string& ip2);
	void remove_edge(std::string& label1, std::string& label2);

	size_t size() { return m_size; }
	bool node_exists(std::string& label) { return m_nodes.count(label) == 1; }
private:

	std::unordered_map<std::string, std::unique_ptr<Node>> m_nodes;
	size_t m_size;
};