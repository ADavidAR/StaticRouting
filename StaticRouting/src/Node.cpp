#include "Node.h"

void Node::add_interface(std::string& iface)
{
	Interface inter(iface);

	std::string label = inter.get_dotted_network_ip_address() + "/" + std::to_string(inter.get_mask_length());
	if (m_interfaces_w_neighbor.count(label))
	{
		set_console_warning();
		std::cout << "[INFO]: Interface existente: " << label << "\n\n";
		reset_console_color();
		return;
	}

	m_neighborless_interfaces[label] = inter;
	m_interface_count++;
}

void Node::add_interface(std::string& ip1, std::string& ip2, Node* neighbor)
{
	Interface inter;
	try
	{
		inter = Interface(ip1, ip2, neighbor);
	}
	catch (const IpException& e)
	{
		throw e;
	}

	if (m_interfaces_w_neighbor.count(neighbor->get_label()))
	{
		set_console_warning();
		std::cout << "[INFO]: Interface existente con nodo: " << neighbor->get_label() << "\n\n";
		reset_console_color();
		return;
	}

	m_interfaces_w_neighbor[neighbor->get_label()] = inter;
	m_interface_count++;
}

void Node::remove_neighborless_interface(std::string& iface, bool formatted)
{
	std::string label = formatted ? iface : Interface::format_ip_address(iface);

	if (!m_neighborless_interfaces.count(label))
	{
		
		set_console_warning();
		std::cout << "[INFO]: Interface inexistente: " << label << "\n\n";
		reset_console_color();
		return;
	}

	m_neighborless_interfaces.erase(label);
	m_interface_count--;
}

void Node::remove_interface_w_neighbor(std::string& label, bool silent)
{
	if (!m_interfaces_w_neighbor.count(label))
	{
		if (!silent)
		{
			set_console_warning();
			std::cout << "[INFO]: Interface inexistente: " << label << "\n\n";
			reset_console_color();
			return;
		}
	}

	m_interfaces_w_neighbor.erase(label);
	m_interface_count--;
}

//void Node::set_interfaces_w_neighbor(std::vector<std::string[2]>& interfaces)
//{
//	m_interfaces_w_neighbor.clear();
//	m_interface_count -= m_interfaces_w_neighbor.size();
//	for (auto iface : interfaces)
//		add_interface(iface[1], iface[0]);
//}

