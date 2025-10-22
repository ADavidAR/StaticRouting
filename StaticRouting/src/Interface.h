#pragma once

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <sstream>
#include <iterator>
#include <bit>

class Node;

#include "Node.h"
#include "IpException.h"

struct Ip
{
    int ip_address;
    short mask_length;
};

class Interface
{
public:
    Interface() : m_ip_address1(0), m_ip_address2(0), m_mask_length(0), m_neighbor_node(nullptr), m_network_ip_address(0) {}
    Interface(std::string& ip);
    Interface(std::string& ip1, std::string& ip2, Node* n);
    int get_local_ip_address() const { return m_ip_address1; }
    int get_neighbor_ip_address() const { return m_ip_address2; }
    auto get_neighbor() const { return m_neighbor_node;  }
    short get_mask_length() const { return m_mask_length; }
    int get_network_ip_address() const { return m_network_ip_address; }
    std::string get_dotted_mask() const;
    std::string get_dotted_network_ip_address() const;
    std::string get_full_dotted_network_ip_address () const;

    static std::string bin_to_dotted(int bits);
    static int dotted_to_bin(std::string& dotted);
    static std::string format_ip_address(std::string& ip);
private:

    Node* m_neighbor_node;
    int m_ip_address1;
    int m_ip_address2;
    short m_mask_length;
    int m_network_ip_address;
    
    static Ip m_string_to_ip(std::string& ip);
};