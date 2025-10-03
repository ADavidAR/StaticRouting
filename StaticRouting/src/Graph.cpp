#include "Graph.h"

#include <iostream>


Node* Graph::add_node(std::string& label)
{
	if (!m_nodes.count(label))
	{
		auto node = std::make_unique<Node>(label);
		
		Node* raw_ptr = node.get();
		
		m_nodes[label] = std::move(node);

		return raw_ptr;
	}
	set_console_warning();
	std::cout << "[INFO]: Nodo existente: " << label << "\n\n";
	reset_console_color();
	return m_nodes[label].get();
	
}

Node* Graph::add_node(Node& node)
{
	if (!m_nodes.count(node.get_label()))
	{
		auto ptr_node = std::make_unique<Node>(node);

		Node* raw_ptr = ptr_node.get();

		m_nodes[node.get_label()] = std::move(ptr_node);

		return raw_ptr;
	}
	set_console_warning();
	std::cout << "[INFO]: Nodo existente: " << node.get_label() << "\n\n";
	reset_console_color();

	return m_nodes[node.get_label()].get();
}

Node* Graph::get_node(std::string& label)
{
	if (node_exists(label))
		return m_nodes[label].get();
	set_console_warning();
	std::cout << "[INFO]: Nodo Inexistente: " << label << "\n\n";
	reset_console_color();
	return nullptr;
}

void Graph::rename_node(std::string& old_label, std::string& new_label)
{
	if (!node_exists(old_label))
	{
		set_console_warning();
		std::cout << "[INFO]: Nodo Inexistente: " << old_label << "\n\n";
		reset_console_color();
		return;
	}

	if (node_exists(new_label))
	{
		set_console_warning();
		std::cout << "[INFO]: Nodo Existente: " << new_label << "\n\n";
		reset_console_color();
		return;
	}

	m_nodes[old_label].get()->set_label(new_label);
}

std::vector<Node*> Graph::nodes()
{
	std::vector<Node*> res;
	for (const auto& [k, v] : m_nodes)
		res.push_back(v.get());

	return res;
}

void Graph::remove_node(std::string& label)
{
	if (!m_nodes.count(label)) return;

	for (auto& [l, node] : m_nodes)
	{
		node->remove_interface_w_neighbor(label);
	}

	m_nodes.erase(label);
}

void Graph::add_edge(std::string& label1, std::string& ip1, std::string& label2, std::string& ip2)
{
	if (!m_nodes.count(label1))
		add_node(label1);

	if (!m_nodes.count(label2))
		add_node(label2);

	try
	{
		m_nodes[label1].get()->add_interface(ip1, ip2, m_nodes[label2].get());
		m_nodes[label2].get()->add_interface(ip2, ip1, m_nodes[label1].get());
	}
	catch (const IpException& e)
	{
		set_console_error();
		std::cout << "[ERROR]: " << e.what() << "\n\n";
		reset_console_color();
	}
}

void Graph::remove_edge(std::string& label1, std::string& label2)
{
	if (!m_nodes.count(label1)) return;
	if (!m_nodes.count(label2)) return;

	m_nodes[label1].get()->remove_interface_w_neighbor(label2);
	m_nodes[label2].get()->remove_interface_w_neighbor(label1);
}